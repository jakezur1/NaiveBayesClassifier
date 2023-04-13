// Project UID db1f506d06d84ab787baf250c265e24e


#include <iostream>
#include <utility>
#include <fstream>
#include <map>
#include <set>
#include <cmath>
#include "csvstream.h"

using namespace std;

class NaiveBayesClassifier {
public:
  NaiveBayesClassifier(string training_filename_in) : training_filename(training_filename_in) {}

  void print_csv_data() {
    csvstream train(training_filename);
    map<string,string> row; 
    while(train >> row) {
      string label = row["tag"];
      string content = row["content"];
      cout << "  label = " << label << ", content = " << content << endl;
    }
  }
  
  void print_test_data(csvstream &test_in) {
    cout << "test data:" << endl;
    map<string, string> row;
    int number_correct = 0;
    int num_test_posts = 0;
    while(test_in >> row) {
      string correct_label = row["tag"];
      string post = row["content"];
      num_test_posts++;
      pair<string,double> prediction = predict(post);
      if (correct_label == prediction.first) {
        number_correct++;
      }
      cout << "  " << "correct = " << correct_label << ", predicted = " 
           << prediction.first << ", log-probability score = " 
           << prediction.second << endl;
      cout << "  " << "content = " << post << endl;
      cout << endl;
    }

    cout << "performance: " << number_correct << " / " << num_test_posts 
         << " posts predicted correctly" << endl;
  }

  void print_without_debug(csvstream & test_in) {
    cout.precision(3);
    cout << "trained on " << num_posts << " examples" << endl << endl;
    print_test_data(test_in);
  }

  void print_with_debug(csvstream & test_in) {
    cout.precision(3);
    calcuations_for_debug();
    cout << "training data:" << endl;
    print_csv_data();
    cout << "trained on " << num_posts << " examples" << endl;
    cout << "vocabulary size = " << num_words << endl << endl;
    cout << "classes:" << endl;
    for (auto iter: num_posts_with_label) {
      cout << "  " << iter.first << ", " << iter.second 
           << " examples," <<  " log-prior = "
           << log_prior_by_label[iter.first] << endl;
    }

    cout << "classifier parameters:" << endl;
    for (auto first_level : num_posts_of_label_with_word) {
      for (auto second_level : first_level.second) {
        cout << "  " << first_level.first << ":" << second_level.first << ", count = " 
             << second_level.second << ", log-likelihood = " 
             << log_likelihood_of_word_in_label[first_level.first][second_level.first] 
             << endl;
      }
    }
    
    cout << endl;
    print_test_data(test_in);
  }
  
  // Gives us the unique words from a string passed in 
  set<string> unique_words(const string &str) {
    istringstream source(str);
    set<string> words;
    string word;
    while (source >> word) {
      words.insert(word);
    }
    return words;
  }

  void update_num_posts_with_word(set<string> updating) {
    for (auto word : updating) {
      num_posts_with_word[word]++;
    }
  }
  
  void update_num_posts_with_label(const string label) {
    num_posts_with_label[label]++;
  }
 
  void update_num_posts_of_label_with_word(const string label, set<string> update) {
      for (auto word : update) {
        num_posts_of_label_with_word[label][word]++;
      }
  }
  
  double calc_log_prior(string label) {
    double log_prior = log(double(num_posts_with_label[label]) / num_posts);
    return log_prior;
  }

  double calc_log_likelihood(string word, string label) {
    double log_likelihood;
    if (num_posts_of_label_with_word[label][word] != 0) {
      log_likelihood = log(double(num_posts_of_label_with_word[label][word]) / 
                       num_posts_with_label[label]);
    } else if (words.find(word) != words.end()) {
      log_likelihood = log(double(num_posts_with_word[word]) / num_posts);
    } else {
      log_likelihood = log(1 / double(num_posts));
    }

    return log_likelihood;
  }

  void calcuations_for_debug() {
    for (auto label : num_posts_of_label_with_word) {
      log_prior_by_label[label.first] = calc_log_prior(label.first);
      for (auto word : label.second) {
        log_likelihood_of_word_in_label[label.first][word.first] = 
          calc_log_likelihood(word.first, label.first);
      }
    }
  }

  // Going to read the data from the CSV file and store the labels and unique words.
  // take in the labels, and then update the three maps that we need
  void fit() {
    csvstream train(training_filename);
    map<string,string> row;
    while(train >> row) {
      string label = row["tag"];
      string content = row["content"];
      labels.insert(label);
      set<string> holding;
      holding = unique_words(row["content"]);
      words.insert(holding.begin(), holding.end());
      update_num_posts_with_label(label);
      update_num_posts_with_word(holding);
      update_num_posts_of_label_with_word(label, holding);
      num_posts++;
    }
    num_words = words.size();
  }

  // predicts the label for the given post
  pair<string,double> predict(string post) { 
    set<string> unique_post = unique_words(post);
    string predicted_label;
    double highest_probability;
    for(auto label : labels) {
      double final_calc = 0;
      double log_prior = calc_log_prior(label);
      double running_log_likelihood = 0.0;
      for (auto word : unique_post) {
        running_log_likelihood += calc_log_likelihood(word, label);
      }
      final_calc = log_prior + running_log_likelihood;
      if (label == *labels.begin() || final_calc > highest_probability) {
        predicted_label = label;
        highest_probability = final_calc;
      }
    }
    return {predicted_label, highest_probability};
  }


private :
  // training filename
  string training_filename;

  // unique values and base metrics of training set
  set<string> labels;
  set<string> words;
  int num_posts = 0;
  int num_words = 0;

  // parameters to learn while training
  map<string, int> num_posts_with_word;
  map<string, int> num_posts_with_label;
  map<string, map<string, int > > num_posts_of_label_with_word;
  map<string, double> log_prior_by_label;
  map<string, map<string, double> > log_likelihood_of_word_in_label;
};


int main(int argc, char *argv[]) {
  cout.precision(3);
  if (argc < 3 || argc > 4) {
    cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
    return 1;
  }
  if (argc == 4 && string(argv[3]) != "--debug") {
    cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
    return 1;
  }

  string training_file = argv[1];
  string testing_file = argv[2];
  csvstream train_in(training_file);
  csvstream test_in(testing_file);
  if (!train_in) {
    cout << "Error opening file: " << training_file << endl;
    return 1;
  }
  if (!test_in) {
    cout << "Error opening file: " << testing_file << endl;
    return 1;
  }
  
  NaiveBayesClassifier nbc(training_file);
  nbc.fit();
  if (argc == 4) {
    nbc.print_with_debug(test_in);
  } else {
    nbc.print_without_debug(test_in);
  }
}
