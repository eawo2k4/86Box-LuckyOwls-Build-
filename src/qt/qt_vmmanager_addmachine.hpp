#ifndef QT_VMMANAGER_ADDMACHINE_H
#define QT_VMMANAGER_ADDMACHINE_H

#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QRadioButton>
#include <QRegularExpression>
#include <QWizard>
#include <QComboBox>

/* =====================================================================
 *  VMManagerAddMachine Wizard
 * ===================================================================== */

class VMManagerAddMachine final : public QWizard {
    Q_OBJECT

public:
    enum {
        Page_Intro,
        Page_WithExistingConfig,
        Page_OSSelect,
        Page_NameAndLocation,
        Page_Conclusion
    };

    explicit VMManagerAddMachine(QWidget *parent = nullptr);
};

/* =====================================================================
 *  Intro Page
 * ===================================================================== */

class IntroPage : public QWizardPage {
    Q_OBJECT

public:
    explicit IntroPage(QWidget *parent = nullptr);
    [[nodiscard]] int nextId() const override;

private:
    QLabel       *topLabel;
    QRadioButton *newConfigRadioButton;
    QRadioButton *existingConfigRadioButton;
};

/* =====================================================================
 *  OS Selection Page (NEW)
 * ===================================================================== */

class OSSelectPage final : public QWizardPage {
    Q_OBJECT

public:
    explicit OSSelectPage(QWidget *parent = nullptr);

    [[nodiscard]] int  nextId() const override;
    [[nodiscard]] bool isComplete() const override;

    QString selectedFamily() const;
    QString selectedVersion() const;

private slots:
    void updateVersionList(const QString &family);
    void updateWarning(const QString &version);

private:
    QLabel    *osFamilyLabel;
    QLabel    *osVersionLabel;
    QComboBox *osFamilyCombo;
    QComboBox *osVersionCombo;
    QLabel    *warningLabel;
};

/* =====================================================================
 *  WithExistingConfigPage
 * ===================================================================== */

class WithExistingConfigPage final : public QWizardPage {
    Q_OBJECT
    Q_PROPERTY(QString configuration READ configuration WRITE setConfiguration NOTIFY configurationChanged)

public:
    explicit WithExistingConfigPage(QWidget *parent = nullptr);

    [[nodiscard]] QString configuration() const;
    void                  setConfiguration(const QString &configuration);

signals:
    void configurationChanged(const QString &configuration);

private:
    QPlainTextEdit *existingConfiguration;

private slots:
    void chooseExistingConfigFile();

protected:
    [[nodiscard]] int  nextId() const override;
    [[nodiscard]] bool isComplete() const override;
};

/* =====================================================================
 *  NameAndLocationPage
 * ===================================================================== */

class NameAndLocationPage final : public QWizardPage {
    Q_OBJECT

public:
    explicit NameAndLocationPage(QWidget *parent = nullptr);
    [[nodiscard]] int nextId() const override;

private:
    QLineEdit *systemName;
    QLineEdit *displayName;
    QLabel    *systemNameValidation;

protected:
    [[nodiscard]] bool isComplete() const override;
    bool               eventFilter(QObject *watched, QEvent *event) override;
};

/* =====================================================================
 *  Conclusion Page
 * ===================================================================== */

class ConclusionPage final : public QWizardPage {
    Q_OBJECT

public:
    explicit ConclusionPage(QWidget *parent = nullptr);

private:
    QLabel *topLabel;
    QLabel *systemName;
    QLabel *displayNameLabel;
    QLabel *displayName;

protected:
    void initializePage() override;
};

#endif // QT_VMMANAGER_ADDMACHINE_H