#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <QObject>

// ThreadPool Klasse
class ThreadPool : public QObject {
  Q_OBJECT

public:
  explicit ThreadPool(size_t numThreads);

  ~ThreadPool();

  void addTask(std::function<void()> task);  // Füge Aufgabe zum Pool hinzu
  void startWorkerThreads();  // Starte die Worker-Threads

  signals:
      void updateGuiSignal(const QString& message);  // Signal für GUI-Updates

private:
  void worker();  // Arbeiter-Thread für Aufgaben
  void guiWorker();  // GUI-Arbeiter-Thread, läuft kontinuierlich

  std::vector<std::thread> workers;
  std::queue<std::function<void()>> tasks;
  std::mutex queueMutex;
  std::condition_variable cv;
  bool stop;
};

#endif // THREADPOOL_H
