#ifndef SETTINGSLIDER_H
#define SETTINGSLIDER_H

#include <QWidget>
#include "tool.h"

namespace Ui {
class SettingSlider;
}

class SettingSlider : public QWidget
{
    Q_OBJECT

public:
    explicit SettingSlider(QWidget *parent = 0);

    ~SettingSlider();

private:
    Ui::SettingSlider *ui;

protected:
    Setting setting;

signals:
    void updateSetting( const Setting& );

public slots:
    void setSetting( const Setting& );

protected slots:
    void updateValueFromSpin();
    void updateValueFromSlide();

};

#endif // SETTINGSLIDER_H
