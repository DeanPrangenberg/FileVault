#include "ThreadPool.h"
#include <iostream>
#include <QTimer>

// Konstruktor: Initialisiere Thread-Pool
ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
  // Starte den GUI-Thread für kontinuierliche Updates
  workers.push_back(std::thread(&ThreadPool::guiWorker, this));

  // Starte die Worker-Threads für parallele Aufgaben
  for (size_t i = 1; i < numThreads; ++i) {
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

// GUI-Arbeiter-Thread für kontinuierliche GUI-Updates
void ThreadPool::guiWorker() {
  while (true) {
    // Beispiel: Alle 100ms GUI-Updates senden
    emit updateGuiSignal("GUI aktualisiert!");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}
