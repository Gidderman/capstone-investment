// The starting point of the application. It constructs a Qt Application, as
// well as the master controller that will direct the rest of the application.

#include "MasterController.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileSystemWatcher>
#include <QTimer>

namespace {

QString styleDir() { return QString::fromUtf8(STYLE_DIR); }

// Returns the text of one file, or an emplty string (with a warning) if it
// can't be opened
QString readFile(const QString &path) {
  QFile file(path);
  if (!file.open(QFile::ReadOnly | QFile::Text)) {
    qWarning() << "Could not open stylesheet:" << path;
    return {};
  }
  return QString::fromUtf8(file.readAll());
}

// Every .qss file in the folder, with common.qss first if it exists
QStringList styleFiles() {
  QDir dir(styleDir());
  QStringList names = dir.entryList({"*.qss"}, QDir::Files, QDir::Name);
  if (names.removeAll("common.qss") > 0) {
    names.prepend("common.qss");
  }

  QStringList paths;
  for (const QString &name : names) {
    paths << dir.filePath(name);
  }
  return paths;
}

// Joins all sheets into one string and applies it to the whole application
void applyStyles(QApplication &app) {
  QString combined;
  for (const QString &path : styleFiles()) {
    combined += readFile(path);
    combined += '\n';
  }
  app.setStyleSheet(combined);
}

// Makes sure the folder and every current .qss file are being watched
void updateWatcher(QFileSystemWatcher &watcher) {
  QStringList toAdd;
  if (!watcher.directories().contains(styleDir())) {
    toAdd << styleDir();
  }
  for (const QString &path : styleFiles()) {
    if (!watcher.files().contains(path)) {
      toAdd << path;
    }
  }
  if (!toAdd.isEmpty()) {
    watcher.addPaths(toAdd);
  }
}

} // namespace

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  applyStyles(app);

  QFileSystemWatcher watcher;
  updateWatcher(watcher);

  // Restarting a single-shot timer on every change batches the burst of event
  // an editor produces during one save into a single reload
  QTimer reloadTimer;
  reloadTimer.setSingleShot(true);
  reloadTimer.setInterval(100);
  QObject::connect(&reloadTimer, &QTimer::timeout, [&]() {
    applyStyles(app);
    updateWatcher(watcher);
  });

  auto scheduleReload = [&]() { reloadTimer.start(); };
  QObject::connect(&watcher, &QFileSystemWatcher::fileChanged, scheduleReload);
  QObject::connect(&watcher, &QFileSystemWatcher::directoryChanged,
                   scheduleReload);

  MasterController masterController = MasterController();
  masterController.executeMainFunctions();

  return app.exec();
}
