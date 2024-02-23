#include <iostream>
#include <random>

int main() {
    // Seed the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

    // Define the range of random numbers
    int min = 1;
    int max = 100;

    // Generate a random number
    std::uniform_int_distribution<> dis(min, max);
    int randomNumber = dis(gen);

    // Print the random number
    std::cout << "Random number: " << randomNumber << std::endl;

    return 0;
}