#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QStyle>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QGridLayout>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QDir>
#include <QRegularExpression>
#include <QKeyEvent>

#include "qt_vmmanager_addmachine.hpp"
#include "AutoProfiles.hpp"

extern "C" {
#include <86box/86box.h>
}

/* =====================================================================
 *  VMManagerAddMachine Wizard
 * ===================================================================== */

VMManagerAddMachine::VMManagerAddMachine(QWidget *parent)
    : QWizard(parent)
{
    setPage(Page_Intro, new IntroPage);
    setPage(Page_WithExistingConfig, new WithExistingConfigPage);
    setPage(Page_OSSelect, new OSSelectPage);
    setPage(Page_NameAndLocation, new NameAndLocationPage);
    setPage(Page_Conclusion, new ConclusionPage);

#ifndef Q_OS_MACOS
    setWizardStyle(ModernStyle);
    setPixmap(LogoPixmap, QPixmap(":assets/addvm-logo.png"));
#else
    setWizardStyle(MacStyle);
    setPixmap(BackgroundPixmap, QPixmap(":/assets/86box-wizard.png"));
#endif

#ifdef Q_OS_WINDOWS
    setMinimumSize(QSize(550, size().height()));
    setMaximumSize(QSize(550, size().height()));
    setWindowFlag(Qt::MSWindowsFixedSizeDialogHint, true);
#else
    setMinimumSize(size());
#endif

    setOption(HaveHelpButton, false);
    setWindowTitle(tr("Add new system wizard"));
}

/* =====================================================================
 *  Intro Page
 * ===================================================================== */

IntroPage::IntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Introduction"));
    setSubTitle(tr("Choose how you want to create the new system."));

    topLabel = new QLabel(tr("Select whether to create a new configuration or use an existing one."));
    topLabel->setWordWrap(true);

    newConfigRadioButton      = new QRadioButton(tr("Create a new configuration"));
    existingConfigRadioButton = new QRadioButton(tr("Use an existing configuration"));
    newConfigRadioButton->setChecked(true);

    auto layout = new QVBoxLayout();
    layout->addWidget(topLabel);
    layout->addWidget(newConfigRadioButton);
    layout->addWidget(existingConfigRadioButton);
    setLayout(layout);
}

int
IntroPage::nextId() const
{
    if (newConfigRadioButton->isChecked())
        return VMManagerAddMachine::Page_OSSelect;

    return VMManagerAddMachine::Page_WithExistingConfig;
}

/* =====================================================================
 *  OS Selection Page (NEW)
 * ===================================================================== */

OSSelectPage::OSSelectPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Operating system"));
    setSubTitle(tr("Select the operating system family and version."));

    auto familyLabel = new QLabel(tr("OS family:"));
    osFamilyCombo    = new QComboBox();
    osFamilyCombo->addItems({ "Microsoft Windows",
                              "Microsoft Windows NT",
                              "Windows 1.x–3.x",
                              "MS-DOS",
                              "IBM PC-DOS",
                              "Linux",
                              "IBM OS/WARP",
                              "Other" });
    registerField("osFamily*", osFamilyCombo);

    auto versionLabel = new QLabel(tr("OS version:"));
    osVersionCombo    = new QComboBox();
    registerField("osVersion*", osVersionCombo);

    warningLabel = new QLabel();
    warningLabel->setVisible(false);
    warningLabel->setWordWrap(true);
    warningLabel->setTextFormat(Qt::RichText);
    warningLabel->setStyleSheet("color: orange; font-style: italic;");

    auto layout = new QGridLayout();
    layout->addWidget(familyLabel, 0, 0);
    layout->addWidget(osFamilyCombo, 0, 1);
    layout->addWidget(versionLabel, 1, 0);
    layout->addWidget(osVersionCombo, 1, 1);
    layout->addWidget(warningLabel, 2, 0, 1, -1);
    setLayout(layout);

    connect(osFamilyCombo, &QComboBox::currentTextChanged,
            this, &OSSelectPage::updateVersionList);

    connect(osVersionCombo, &QComboBox::currentTextChanged,
            this, &OSSelectPage::updateWarning);

    updateVersionList(osFamilyCombo->currentText());
}

void
OSSelectPage::updateVersionList(const QString &family)
{
    QSignalBlocker block(osVersionCombo);
    osVersionCombo->clear();
    warningLabel->setVisible(false);

    if (family == "Microsoft Windows") {
        osVersionCombo->addItems({ "Windows 95", "Windows 98", "Windows ME" });

    } else if (family == "Microsoft Windows NT") {
        osVersionCombo->addItems({ "Windows NT 3.1", "Windows NT 3.5",
                                   "Windows NT 4.0", "Windows 2000",
                                   "Windows XP" });

    } else if (family == "Windows 1.x–3.x") {
        osVersionCombo->addItems({ "Windows 1.0", "Windows 2.0",
                                   "Windows 3.0", "Windows 3.1" });

    } else if (family == "MS-DOS") {
        osVersionCombo->addItems({ "MS-DOS 3.3", "MS-DOS 5.0", "MS-DOS 6.22" });

    } else if (family == "IBM PC-DOS") {
        osVersionCombo->addItems({ "PC-DOS 3.3", "PC-DOS 5.0", "PC-DOS 7.0" });

    } else if (family == "Linux") {
        osVersionCombo->addItems({ "Debian", "Slackware", "Red Hat 5.2" });

    } else if (family == "IBM OS/WARP") {
        osVersionCombo->addItems({ "OS/2 Warp 3", "OS/2 Warp 4" });

    } else if (family == "Other") {
        osVersionCombo->setEnabled(false);
        emit completeChanged();
        return;
    }

    osVersionCombo->setEnabled(true);

    if (osVersionCombo->count() > 0)
        osVersionCombo->setCurrentIndex(0);

    emit completeChanged();
}

void
OSSelectPage::updateWarning(const QString &version)
{
    if (version == "Windows XP") {
        warningLabel->setVisible(true);
        warningLabel->setText(
            "<img src=':/misc/qt/icons/warning.ico' width='16' height='16' "
            "style='vertical-align:middle; margin-right:6px;'>"
            "<span style='color: orange; font-style: italic;'>"
            "Windows XP will run very slowly on Pentium II emulation."
            "</span>");
    } else {
        warningLabel->setVisible(false);
    }
}

bool
OSSelectPage::isComplete() const
{
    if (osFamilyCombo->currentText() == "Other")
        return true;

    return osFamilyCombo->currentIndex() >= 0 && osVersionCombo->currentIndex() >= 0;
}

int
OSSelectPage::nextId() const
{
    return VMManagerAddMachine::Page_NameAndLocation;
}

/* =====================================================================
 *  WithExistingConfigPage
 * ===================================================================== */

WithExistingConfigPage::WithExistingConfigPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Use existing configuration"));
    setSubTitle(tr("Paste the contents of the existing configuration file."));

    existingConfiguration = new QPlainTextEdit();

    auto font = QFont();
    font.setFamily("Consolas");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    existingConfiguration->setFont(font);

    connect(existingConfiguration, &QPlainTextEdit::textChanged,
            this, &WithExistingConfigPage::completeChanged);

    registerField("existingConfiguration*", this, "configuration");

    auto layout = new QVBoxLayout();
    layout->addWidget(existingConfiguration);

    auto loadButton = new QPushButton();
    auto loadLabel  = new QLabel(tr("Load configuration from file"));
    auto h          = new QHBoxLayout();

    loadButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileIcon));
    loadButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

    connect(loadButton, &QPushButton::clicked,
            this, &WithExistingConfigPage::chooseExistingConfigFile);

    h->addWidget(loadButton);
    h->addWidget(loadLabel);
    layout->addLayout(h);

    setLayout(layout);
}

void
WithExistingConfigPage::chooseExistingConfigFile()
{
    const auto start = QString(vmm_path);
    const auto file  = QFileDialog::getOpenFileName(
        this,
        tr("Choose configuration file"),
        start,
        tr("86Box configuration files (86box.cfg)"));

    if (file.isEmpty())
        return;

    QFile cfg(file);
    if (!cfg.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(
            this,
            tr("Configuration read failed"),
            tr("Unable to open file: %1").arg(cfg.errorString()));
        return;
    }

    existingConfiguration->setPlainText(cfg.readAll());
    cfg.close();
    emit completeChanged();
}

QString
WithExistingConfigPage::configuration() const
{
    return existingConfiguration->toPlainText();
}

void
WithExistingConfigPage::setConfiguration(const QString &c)
{
    if (c != existingConfiguration->toPlainText()) {
        existingConfiguration->setPlainText(c);
        emit configurationChanged(c);
    }
}

int
WithExistingConfigPage::nextId() const
{
    return VMManagerAddMachine::Page_NameAndLocation;
}

bool
WithExistingConfigPage::isComplete() const
{
    return !existingConfiguration->toPlainText().isEmpty();
}

/* =====================================================================
 *  NameAndLocationPage
 * ===================================================================== */

NameAndLocationPage::NameAndLocationPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("System name"));
    setSubTitle(tr("Enter the name of the system"));

    auto nameLabel = new QLabel(tr("System name:"));
    systemName     = new QLineEdit();
    systemName->installEventFilter(this);
    registerField("systemName*", systemName);

    systemNameValidation = new QLabel();

    auto displayLabel = new QLabel(tr("Display name (optional):"));
    displayName       = new QLineEdit();
    displayName->installEventFilter(this);
    registerField("displayName*", displayName);

    auto layout = new QGridLayout();
    layout->setRowMinimumHeight(1, 20);

    layout->addWidget(nameLabel, 2, 0);
    layout->addWidget(systemName, 2, 1);
    layout->addWidget(systemNameValidation, 3, 0, 1, -1);

    layout->setRowMinimumHeight(7, 20);
    layout->addWidget(displayLabel, 8, 0);
    layout->addWidget(displayName, 8, 1);

    setLayout(layout);
}

int
NameAndLocationPage::nextId() const
{
    return VMManagerAddMachine::Page_Conclusion;
}

bool
NameAndLocationPage::isComplete() const
{
    if (systemName->text().isEmpty()) {
        systemNameValidation->setText(tr("Please enter a system name"));
        return false;
    }

    systemNameValidation->clear();
    return true;
}

bool
NameAndLocationPage::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        auto key = static_cast<QKeyEvent *>(event);
        if (key->key() == Qt::Key_Enter || key->key() == Qt::Key_Return) {
            if (auto wiz = qobject_cast<QWizard *>(wizard())) {
                if (wiz->currentPage()->isComplete())
                    wiz->next();
            }
            return true;
        }
    }
    return QWizardPage::eventFilter(watched, event);
}

/* =====================================================================
 *  Conclusion Page
 * ===================================================================== */

ConclusionPage::ConclusionPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Complete"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":assets/addvm-watermark.png"));

    topLabel = new QLabel(tr("The wizard will now launch the configuration for the new system."));
    topLabel->setWordWrap(true);

    auto nameLabel = new QLabel(tr("System name:"));
    nameLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

    systemName = new QLabel();
    systemName->setWordWrap(true);

    displayNameLabel = new QLabel(tr("Display name:"));
    displayNameLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

    displayName = new QLabel();
    displayName->setWordWrap(true);

    auto layout = new QGridLayout();
    layout->addWidget(topLabel, 0, 0, 1, -1);
    layout->setRowMinimumHeight(1, 20);

    layout->addWidget(nameLabel, 2, 0);
    layout->addWidget(systemName, 2, 1);

    layout->addWidget(displayNameLabel, 4, 0);
    layout->addWidget(displayName, 4, 1);

    setLayout(layout);
}

void
ConclusionPage::initializePage()
{
    const auto name  = field("systemName").toString();
    const auto dname = field("displayName").toString();

    systemName->setText(name);

    if (!dname.isEmpty()) {
        displayNameLabel->setVisible(true);
        displayName->setVisible(true);
        displayName->setText(dname);
    } else {
        displayNameLabel->setVisible(false);
        displayName->setVisible(false);
    }
}