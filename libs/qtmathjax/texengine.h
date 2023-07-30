#ifndef TEXENGINE_H
#define TEXENGINE_H
#include <QCoreApplication>
#include <QWebElement>
#include <QWebFrame>
#include <QWebView>
#include <QCache>
#include <QDir>

class TeXEngine: QObject {
    Q_OBJECT
public:
    explicit TeXEngine (QWebView *webView = nullptr);
    ~TeXEngine ();
    // compute immediately, waiting for result (via processEvents()) before returning it:
    QString TeX2SVG ( QString TeXcode, bool forceReload = false );
    // queue computation for later iff it hasn't yet been done; cache result when done:
    void asyncTeX2SVG ( QString TeXcode );
    // detect whether a string has already been compiled in the past (i.e., is in cache):
    bool hasComputed (QString TeXcode);
    QString error ();
    Q_INVOKABLE void MathJaxDone ();
    Q_INVOKABLE void MathJaxError ( QString errorMessage );
    QWebView* webView ();
private:
    QWebView* view;
    QWebFrame* frame;
    QString lastError;
    bool running;
    bool isReady;
    QCache<QString,QString> cache;
    QStringList queue;
    QString currentInput;
    void computeNextInBackground ();
private slots:
    void addJSObject ();
    void ready ( bool loadSucceeded );
};


#endif // TEXENGINE_H

