#include "settingslider.h"
#include "ui_settingslider.h"

#define MAXSLIDER 1000.0

SettingSlider::SettingSlider(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingSlider)
{
    ui->setupUi(this);
    ui->horizontalSlider->setMinimum(0);
    ui->horizontalSlider->setMaximum(MAXSLIDER);
}

void SettingSlider::setSetting(const Setting &s)
{
    setting = s;
    ui->label->setText( s.name );

    float d = s.max - s.min;

    ui->horizontalSlider->setValue( (s.val - s.min) / d * MAXSLIDER );

    ui->doubleSpinBox->setMinimum(s.min);
    ui->doubleSpinBox->setMaximum(s.max);
    ui->doubleSpinBox->setValue(s.val);
    ui->doubleSpinBox->setSingleStep(d/MAXSLIDER);
}

void SettingSlider::updateValueFromSpin()
{
    #define s setting
    s.val = ui->doubleSpinBox->value();
    ui->horizontalSlider->setValue( (s.val - s.min) / (s.max - s.min) * MAXSLIDER );
    updateSetting(s);
    #undef s
}

void SettingSlider::updateValueFromSlide()
{
    #define s setting
    s.val = ui->horizontalSlider->value() / MAXSLIDER * (s.max - s.min) + s.min;
    ui->doubleSpinBox->setValue(s.val);
    updateSetting(s);
    #undef s
}

SettingSlider::~SettingSlider()
{
    delete ui;
}
