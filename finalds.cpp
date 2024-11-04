#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iomanip>

// Node structure for a linked list
struct Node {
    std::string data;
    Node* next;
    Node(std::string val) : data(val), next(nullptr) {}
};

// Linked list for storing CSV columns or rows
class LinkedList {
public:
    Node* head;
    LinkedList() : head(nullptr) {}

    void append(const std::string& data) {
        if (!head) {
            head = new Node(data);
        } else {
            Node* temp = head;
            while (temp->next) temp = temp->next;
            temp->next = new Node(data);
        }
    }

    std::vector<double> toNumericVector() const {
        std::vector<double> nums;
        Node* temp = head;
        while (temp) {
            try {
                nums.push_back(std::stod(temp->data));
            } catch (...) {
                std::cerr << "Non-numeric data encountered: " << temp->data << '\n';
            }
            temp = temp->next;
        }
        return nums;
    }

    void display() const {
        Node* temp = head;
        while (temp) {
            std::cout << temp->data << " -> ";
            temp = temp->next;
        }
        std::cout << "NULL\n";
    }
};

// Function to load CSV into a vector of linked lists
std::vector<LinkedList> loadCSV(const std::string& filename) {
    std::vector<LinkedList> columns;
    std::ifstream file(filename);
    std::string line, cell;

    if (file.is_open()) {
        while (getline(file, line)) {
            std::stringstream lineStream(line);
            int colIndex = 0;

            while (getline(lineStream, cell, ',')) {
                if (columns.size() <= colIndex) columns.emplace_back();
                columns[colIndex].append(cell);
                colIndex++;
            }
        }
        file.close();
    } else {
        std::cerr << "Unable to open file\n";
    }
    return columns;
}

// Helper functions for mean, median, and mode calculations
double calculateMean(const std::vector<double>& nums) {
    if (nums.empty()) return 0;
    double sum = 0;
    for (double num : nums) sum += num;
    return sum / nums.size();
}

double calculateMedian(std::vector<double> nums) {
    if (nums.empty()) return 0;
    std::sort(nums.begin(), nums.end());
    size_t size = nums.size();
    return size % 2 == 0 ? (nums[size / 2 - 1] + nums[size / 2]) / 2.0 : nums[size / 2];
}

double calculateMode(const std::vector<double>& nums) {
    if (nums.empty()) return 0;
    std::map<double, int> frequency;
    for (double num : nums) frequency[num]++;
    
    int max_count = 0;
    double mode = nums[0];
    for (const auto& pair : frequency) {
        if (pair.second > max_count) {
            max_count = pair.second;
            mode = pair.first;
        }
    }
    return mode;
}

// Menu-based operations
void displayMenu() {
    std::cout << "\nMenu:\n";
    std::cout << "1. Display CSV columns\n";
    std::cout << "2. Calculate Mean of a Column\n";
    std::cout << "3. Calculate Median of a Column\n";
    std::cout << "4. Calculate Mode of a Column\n";
    std::cout << "5. Exit\n";
    std::cout << "Enter your choice: ";
}

void performOperation(const std::vector<LinkedList>& columns, int choice) {
    int col;
    std::cout << "Enter column number: ";
    std::cin >> col;

    if (col <= 0 || col > columns.size()) {
        std::cerr << "Invalid column number\n";
        return;
    }

    std::vector<double> nums = columns[col - 1].toNumericVector();
    if (nums.empty()) {
        std::cerr << "Column contains no numeric data\n";
        return;
    }

    switch (choice) {
        case 2:
            std::cout << "Mean: " << calculateMean(nums) << '\n';
            break;
        case 3:
            std::cout << "Median: " << calculateMedian(nums) << '\n';
            break;
        case 4:
            std::cout << "Mode: " << calculateMode(nums) << '\n';
            break;
        default:
            std::cerr << "Invalid operation\n";
    }
}

int main() {
    std::string filename;
    std::cout << "Enter the path to the CSV file: ";
    std::getline(std::cin, filename);

    // Load the CSV file
    std::vector<LinkedList> columns = loadCSV(filename);

    if (columns.empty()) {
        std::cout << "No data found in CSV or failed to load the file.\n";
        return 1;
    }

    int choice;
    do {
        displayMenu();
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "CSV File Content:\n";
                for (size_t i = 0; i < columns.size(); ++i) {
                    std::cout << "Column " << i + 1 << ":\n";
                    columns[i].display();
                }
                break;
            case 2:
            case 3:
            case 4:
                performOperation(columns, choice);
                break;
            case 5:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cerr << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);

    return 0;
}
