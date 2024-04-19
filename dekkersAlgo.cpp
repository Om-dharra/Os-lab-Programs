#include <iostream>
#include <thread>
#include <atomic>
#include <semaphore>

std::atomic<int> flag0{0}, flag1{0};
std::atomic<int> turn{0};
sem_t sem;

void process0() {
    while (true) {
        flag0 = 1;
        turn = 1;
        while (flag1 == 1 && turn == 1) {
            // Wait
        }
        
        // Critical section
        std::cout << "Process 0 is in the critical section.\n";
        
        flag0 = 0;
        
        // Non-critical section
        std::cout << "Process 0 is in the non-critical section.\n";
        
        // Sleep to avoid busy waiting
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void process1() {
    while (true) {
        flag1 = 1;
        turn = 0;
        while (flag0 == 1 && turn == 0) {
            // Wait
        }
        
        // Critical section
        std::cout << "Process 1 is in the critical section.\n";
        
        flag1 = 0;
        
        // Non-critical section
        std::cout << "Process 1 is in the non-critical section.\n";
        
        // Sleep to avoid busy waiting
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

int main() {
    // Initialize semaphore
    sem_init(&sem, 0, 1);

    // Create threads
    std::thread t0(process0);
    std::thread t1(process1);

    // Join threads
    t0.join();
    t1.join();

    // Destroy semaphore
    sem_destroy(&sem);

    return 0;
}
