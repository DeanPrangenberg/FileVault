#include "ThreadPool.h"
#include <iostream>

// Konstruktor: Initialisiere Thread-Pool
ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
  // Starte den GUI-Thread für kontinuierliche Updates
  // Starte die Worker-Threads für parallele Aufgaben
  for (size_t i = 0; i < numThreads; ++i) {
    workers.push_back(std::thread(&ThreadPool::worker, this));
  }
}

// Destruktor: Stoppe alle Threads
ThreadPool::~ThreadPool() {
  {
    std::lock_guard<std::mutex> lock(queueMutex);
    stop = true;
  }
  cv.notify_all();
  for (auto& worker : workers) {
    if (worker.joinable()) {
      worker.join();
    }
  }
}

// Aufgabe hinzufügen
void ThreadPool::addTask(std::function<void()> task) {
  {
    std::lock_guard<std::mutex> lock(queueMutex);
    tasks.push(task);
  }
  cv.notify_one();
}

// Worker-Thread, der Aufgaben ausführt
void ThreadPool::worker() {
  while (true) {
    std::function<void()> task;
    {
      std::unique_lock<std::mutex> lock(queueMutex);
      cv.wait(lock, [this]() { return stop || !tasks.empty(); });
      if (stop && tasks.empty()) return;
      task = tasks.front();
      tasks.pop();
    }
    task();  // Aufgabe ausführen
  }
}
