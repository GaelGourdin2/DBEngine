#pragma once

#include "BTree.h"
#include <chrono>
#include <random>
#include <algorithm>
#include <iomanip>

using namespace std;
using namespace chrono;

class PerformanceTest {
private:
    vector<int> generate_random_data(int size, int seed = 42) {
        vector<int> data(size);
        mt19937 gen(seed);
        uniform_int_distribution<> dis(1, size * 10);
        
        for (int i = 0; i < size; i++) {
            data[i] = dis(gen);
        }
        return data;
    }
    
    vector<int> generate_sequential_data(int size) {
        vector<int> data(size);
        for (int i = 0; i < size; i++) {
            data[i] = i + 1;
        }
        return data;
    }
    
    void print_test_header(const string& test_name) {
        cout << "\n" << string(60, '=') << endl;
        cout << test_name << endl;
        cout << string(60, '=') << endl;
    }
    
    void print_result(const string& operation, int n, double time_ms, double ops_per_sec) {
        cout << fixed << setprecision(2);
        cout << setw(25) << left << operation;
        cout << "N=" << setw(8) << left << n;
        cout << "Time: " << setw(10) << right << time_ms << " ms  ";
        cout << "Throughput: " << setw(12) << right << (int)ops_per_sec << " ops/sec" << endl;
    }

public:
    void test_insertion_performance(int t_value, int n) {
        print_test_header("Insertion Performance Test (t=" + to_string(t_value) + ")");
        
        // Test 1: Sequential insertion
        {
            BTree tree(t_value);
            vector<int> data = generate_sequential_data(n);
            
            auto start = high_resolution_clock::now();
            for (int val : data) {
                tree.insert(val);
            }
            auto end = high_resolution_clock::now();
            
            double time_ms = duration_cast<microseconds>(end - start).count() / 1000.0;
            double ops_per_sec = (n / time_ms) * 1000.0;
            print_result("Sequential Insert", n, time_ms, ops_per_sec);
        }
        
        // Test 2: Random insertion
        {
            BTree tree(t_value);
            vector<int> data = generate_random_data(n);
            
            auto start = high_resolution_clock::now();
            for (int val : data) {
                tree.insert(val);
            }
            auto end = high_resolution_clock::now();
            
            double time_ms = duration_cast<microseconds>(end - start).count() / 1000.0;
            double ops_per_sec = (n / time_ms) * 1000.0;
            print_result("Random Insert", n, time_ms, ops_per_sec);
        }
        
        // Test 3: Reverse order insertion
        {
            BTree tree(t_value);
            vector<int> data = generate_sequential_data(n);
            reverse(data.begin(), data.end());
            
            auto start = high_resolution_clock::now();
            for (int val : data) {
                tree.insert(val);
            }
            auto end = high_resolution_clock::now();
            
            double time_ms = duration_cast<microseconds>(end - start).count() / 1000.0;
            double ops_per_sec = (n / time_ms) * 1000.0;
            print_result("Reverse Insert", n, time_ms, ops_per_sec);
        }
    }
    
    void test_search_performance(int t_value, int n, int num_searches) {
        print_test_header("Search Performance Test (t=" + to_string(t_value) + ")");
        
        // Build tree with random data
        BTree tree(t_value);
        vector<int> data = generate_random_data(n);
        
        cout << "Building tree with " << n << " elements..." << endl;
        for (int val : data) {
            tree.insert(val);
        }
        
        // Test 1: Search for existing elements
        {
            vector<int> search_keys(num_searches);
            mt19937 gen(123);
            uniform_int_distribution<> dis(0, data.size() - 1);
            
            for (int i = 0; i < num_searches; i++) {
                search_keys[i] = data[dis(gen)];
            }
            
            int found = 0;
            auto start = high_resolution_clock::now();
            for (int key : search_keys) {
                if (tree.search(key) != nullptr) {
                    found++;
                }
            }
            auto end = high_resolution_clock::now();
            
            double time_ms = duration_cast<microseconds>(end - start).count() / 1000.0;
            double ops_per_sec = (num_searches / time_ms) * 1000.0;
            print_result("Search (Hit)", num_searches, time_ms, ops_per_sec);
            cout << "  Found: " << found << "/" << num_searches << endl;
        }
        
        // Test 2: Search for non-existing elements
        {
            vector<int> search_keys(num_searches);
            for (int i = 0; i < num_searches; i++) {
                search_keys[i] = n * 100 + i; // Values not in tree
            }
            
            int found = 0;
            auto start = high_resolution_clock::now();
            for (int key : search_keys) {
                if (tree.search(key) != nullptr) {
                    found++;
                }
            }
            auto end = high_resolution_clock::now();
            
            double time_ms = duration_cast<microseconds>(end - start).count() / 1000.0;
            double ops_per_sec = (num_searches / time_ms) * 1000.0;
            print_result("Search (Miss)", num_searches, time_ms, ops_per_sec);
            cout << "  Found: " << found << "/" << num_searches << endl;
        }
    }
    
    void test_different_t_values(int n) {
        print_test_header("Comparison of Different t Values");
        
        vector<int> t_values = {2, 3, 5, 10, 20, 50, 100};
        vector<int> data = generate_random_data(n);
        
        cout << setw(8) << "t" << setw(15) << "Insert (ms)" << setw(15) << "Search (ms)" << endl;
        cout << string(40, '-') << endl;
        
        for (int t : t_values) {
            // Insertion test
            BTree tree(t);
            auto start = high_resolution_clock::now();
            for (int val : data) {
                tree.insert(val);
            }
            auto end = high_resolution_clock::now();
            double insert_time = duration_cast<microseconds>(end - start).count() / 1000.0;
            
            // Search test
            int num_searches = min(10000, n);
            start = high_resolution_clock::now();
            for (int i = 0; i < num_searches; i++) {
                tree.search(data[i % data.size()]);
            }
            end = high_resolution_clock::now();
            double search_time = duration_cast<microseconds>(end - start).count() / 1000.0;
            
            cout << fixed << setprecision(2);
            cout << setw(8) << t << setw(15) << insert_time << setw(15) << search_time << endl;
        }
    }
    
    void test_scalability() {
        print_test_header("Scalability Test");
        
        vector<int> sizes = {1000, 5000, 10000, 50000, 100000};
        int t = 10;
        
        cout << "Testing with t=" << t << endl;
        cout << setw(12) << "Size" << setw(18) << "Insert (ms)" << setw(18) << "Avg per op (μs)" << endl;
        cout << string(50, '-') << endl;
        
        for (int size : sizes) {
            BTree tree(t);
            vector<int> data = generate_random_data(size);
            
            auto start = high_resolution_clock::now();
            for (int val : data) {
                tree.insert(val);
            }
            auto end = high_resolution_clock::now();
            
            double time_ms = duration_cast<microseconds>(end - start).count() / 1000.0;
            double avg_per_op = (time_ms * 1000.0) / size;
            
            cout << fixed << setprecision(2);
            cout << setw(12) << size << setw(18) << time_ms << setw(18) << avg_per_op << endl;
        }
    }
    
    void run_all_tests() {
        cout << "\n";
        cout << "╔════════════════════════════════════════════════════════════╗\n";
        cout << "║          B-TREE PERFORMANCE TEST SUITE                     ║\n";
        cout << "╚════════════════════════════════════════════════════════════╝\n";
        
        int N = 1000000;
        // Run tests with different parameters
        test_insertion_performance(3, N);
        test_insertion_performance(10, N);
        test_insertion_performance(50, N);
        
        test_search_performance(10, 50000, N);
        
        test_different_t_values(20000);
        
        test_scalability();
        
        cout << "\n" << string(60, '=') << endl;
        cout << "All tests completed!" << endl;
        cout << string(60, '=') << endl;
    }
};
