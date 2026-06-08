#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <string>

using namespace std;

// OOP Approach: Define what a user session looks like
struct UserSession {
    int user_id;
    char test_group; // 'A' for Control, 'B' for Variant
    bool made_purchase;
    int time_spent_sec;
    bool fraud_flag;
};

int main() {
    int total_users = 20000;
    vector<UserSession> dataset;
    
    // Set up random number generators
    random_device rd;
    mt19937 gen(rd());
    
    // Probabilities for Group A (Control - Forced Account)
    bernoulli_distribution purchase_prob_A(0.030); // 3.0% conversion
    bernoulli_distribution fraud_prob_A(0.001);    // 0.1% fraud risk
    normal_distribution<> time_dist_A(180, 45);    // Avg 180 seconds to checkout
    
    // Probabilities for Group B (Variant - Guest Checkout)
    bernoulli_distribution purchase_prob_B(0.038); // 3.8% conversion (our success!)
    bernoulli_distribution fraud_prob_B(0.006);    // 0.6% fraud risk (the trade-off)
    normal_distribution<> time_dist_B(90, 25);     // Avg 90 seconds (much faster)

    cout << "Generating " << total_users << " simulated user sessions..." << endl;

    for (int i = 1; i <= total_users; ++i) {
        UserSession session;
        session.user_id = i;
        
        // 50/50 Split for A/B Test
        if (i % 2 != 0) {
            session.test_group = 'A';
            session.made_purchase = purchase_prob_A(gen);
            session.time_spent_sec = max(30, (int)time_dist_A(gen)); // Minimum 30s
            session.fraud_flag = session.made_purchase ? fraud_prob_A(gen) : false;
        } else {
            session.test_group = 'B';
            session.made_purchase = purchase_prob_B(gen);
            session.time_spent_sec = max(30, (int)time_dist_B(gen));
            session.fraud_flag = session.made_purchase ? fraud_prob_B(gen) : false;
        }
        
        dataset.push_back(session);
    }

    // Export the data to a CSV file
    ofstream outfile("ab_test_results.csv");
    outfile << "user_id,test_group,made_purchase,time_spent_sec,fraud_flag\n";
    
    for (const auto& session : dataset) {
        outfile << session.user_id << ","
                << session.test_group << ","
                << session.made_purchase << ","
                << session.time_spent_sec << ","
                << session.fraud_flag << "\n";
    }
    
    outfile.close();
    cout << "Simulation complete! Data saved to 'ab_test_results.csv'" << endl;

    return 0;
}
