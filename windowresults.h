#ifndef WINDOWRESULTS_H
#define WINDOWRESULTS_H

#include <QWidget>
#include <QMainWindow>
#include <QStatusBar>
#include <QCheckBox>
#include <QMessageBox>
#include <QFutureWatcher>
#include <QProgressDialog>
#include <QThread>
#include <QtConcurrent/QtConcurrentRun>

#include "Target.h"
#include "ui_windowresults.h"
#include "QTableWidget"
#include "QTableWidgetItem"
#include "QListWidgetItem"
#include "utils.h"
#include "targetFinder.h"
#include "corelcoef.h"
#include "export.h"
#include "settings.h"
#include "windowsettings.h"

#include <vector>

class widgetItemCheckBox: public QWidget
{
Q_OBJECT
private:
    int indiceVector;
public:
    int getIndiceVector() const;
    void setIndiceVector(int _indice);

};

class myListItem: public QObject, public QTableWidgetItem
 {
     Q_OBJECT
 public:
     bool operator<(const QTableWidgetItem &other)const;
     //myListItem * myClone();
 };

namespace Ui {
class WindowResults;
}

class WindowResults : public QWidget
{
    Q_OBJECT

public:
    explicit WindowResults(QWidget *parent = 0);
    WindowResults(TargetFinder & p_vTargets, QString const& p_sFileName, QWidget *parent = 0, TargetFinder const& p_vTargetsShuffle=TargetFinder());
    ~WindowResults();
public slots:
    void d(int v);
    void export_finished();
private slots:
    void on_Chb_shouldDisplayResult_stateChanged(int state);
    void on_exportButton_clicked();
    void on_button_optimize_CC_clicked();

    void on_button_export_CC_clicked();

    void on_settingsButton_clicked();

private:
    Ui::WindowResults *ui;
    std::vector<Target> m_vTargets;
    std::vector<Target> m_vTargetsShuffle;
    QString m_sFileName;
    int m_iNbSequences;
    bool m_bFailExport;
    bool m_bIsCC;

    double bestCutoff;
    static const std::string CONST_EXPORT_SUFFIX;
    static const std::string CONST_EXPORT_EXTENSION;
    static const std::string CONST_DEFAULT_NO_CC_STRING;

    QFutureWatcher<void> FutureWatcher;
    QFuture<bool> m_oFutureExport;
    QProgressDialog * ProgressDialog;

    bool myExport(std::string const& p_sFileName);

    void insertRow(int line);
    static QWidget * getCenteredWidget();
    void fillFirstResultTab(double cutoff=Settings::getSettings()->getResults_minimumCutoff());
    double getSelectedCutoff() const;
    void setMaximumCutoffWithResults();
    void optimizeCC();
    double getCC(double p_dCutoff) const;
};

#endif // WINDOWRESULTS_H
