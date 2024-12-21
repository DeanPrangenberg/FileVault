#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

// ThreadPool Klasse
class ThreadPool {

public:
  explicit ThreadPool(size_t numThreads);
  ~ThreadPool();

  // Füge Aufgabe zum Pool hinzu
  void addTask(std::function<void()> task);

private:
  void worker();  // Arbeiter-Thread für Aufgaben

  std::vector<std::thread> workers;
  std::queue<std::function<void()>> tasks;
  std::mutex queueMutex;
  std::condition_variable cv;
  bool stop;
};

#endif // THREADPOOL_H