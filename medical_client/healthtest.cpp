#include "healthtest.h"
#include "ui_healthtest.h"

healthtest::healthtest(QTcpSocket *socket, User *user, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::healthtest)
{
    ui->setupUi(this);
    this->socket = socket;
    this->user = user;
}

healthtest::~healthtest()
{
    delete ui;
}

void healthtest::on_healthtest_return_clicked()
{
    PatientProfile *patfil = new PatientProfile(socket, user);
    patfil->setWindowTitle(QString("患者界面"));
    patfil->show();
    this->hide();
}


//健康评测函数
QString healthtest::Health_Test(int heightint, int weightint,
                                int heart_rateint, int pressureint_height,
                                int pressureint_low, int vital_capacityint,
                                const QString& sexstr, int temperatureint,
                                bool heightok, bool weightok, bool heartok,
                                bool presshighok, bool presslowok,
                                bool vitalok, bool temperaok)
{
    QString assessment;

    // 性别验证
    QString Sexstr = sexstr.trimmed();
    if (Sexstr != "男" && Sexstr != "女") {
        assessment = "性别输入无效，请指定“男”或“女”。";
        return assessment;
    }

    // 参数有效性验证
    if (!heightok) {
        assessment = "身高请输入整数。";
        return assessment;
    }
    if (!weightok) {
        assessment = "体重请输入整数。";
        return assessment;
    }
    if (!heartok) {
        assessment = "心率请输入整数。";
        return assessment;
    }
    if (!presshighok) {
        assessment = "血压高压请输入整数。";
        return assessment;
    }
    if (!presslowok) {
        assessment = "血压低压请输入整数。";
        return assessment;
    }
    if (!vitalok) {
        assessment = "肺活量请输入整数。";
        return assessment;
    }
    if (!temperaok) {
        assessment = "体温请输入整数。";
        return assessment;
    }

    double heightInMeters = heightint / 100.0; // 将身高转换为米
    double bmi = weightint / (heightInMeters * heightInMeters);

    // 身高评估
    switch (Sexstr == "男" ? 1 : 2) {
        case 1:  // 男性
            if (heightint < 170) {
                assessment += "您的身高低于平均水平，建议适量增加营养并保持适度运动。";
            } else if (heightint >= 170 && heightint <= 185) {
                assessment += "您的身高在正常范围内，保持当前的生活方式。";
            } else {
                assessment += "您的身高高于平均水平，注意保持健康的生活方式。";
            }
            break;
        case 2:  // 女性
            if (heightint < 160) {
                assessment += "您的身高低于平均水平，建议适量增加营养并保持适度运动。";
            } else if (heightint >= 160 && heightint <= 175) {
                assessment += "您的身高在正常范围内，保持当前的生活方式.";
            } else {
                assessment += "您的身高高于平均水平，注意保持健康的生活方式.";
            }
            break;
    }

    // 体重评估
    switch (Sexstr == "男" ? 1 : 2) {
        case 1:
            if (bmi < 18.5) {
                assessment += "您的体质指数 (BMI) 为 " + QString::number(bmi, 'f', 1) + "，属于体重过轻类别。建议增加高质量蛋白质和碳水化合物的摄入，并定期进行力量训练.";
            } else if (bmi >= 18.5 && bmi < 24.9) {
                assessment += "您的体质指数 (BMI) 为 " + QString::number(bmi, 'f', 1) + "，属于正常范围，继续保持健康的饮食和适当的运动.";
            } else if (bmi >= 24.9 && bmi < 30.0) {
                assessment += "您的体质指数 (BMI) 为 " + QString::number(bmi, 'f', 1) + "，属于超重类别。建议调整饮食结构，减少高脂肪食物的摄入，并增加有氧运动.";
            } else {
                assessment += "您的体质指数(BMI) 为 " + QString::number(bmi, 'f', 1) + "，属于肥胖类别。强烈建议寻求医生或营养师的指导，制定减重计划，并密切关注体重的变化.";
            }
            break;
        case 2:
            if (bmi < 18.5) {
                assessment += "您的体质指数 (BMI) 为 " + QString::number(bmi, 'f', 1) + "，属于体重过轻类别。建议增加营养摄入，并定期进行健康的体力活动。";
            } else if (bmi >= 18.5 && bmi < 24.9) {
                assessment += "您的体质指数 (BMI) 为 " + QString::number(bmi, 'f', 1) + "，属于正常范围，继续保持均衡饮食和适量运动。";
            } else if (bmi >= 24.9 && bmi < 30.0) {
                assessment += "您的体质指数 (BMI) 为 " + QString::number(bmi, 'f', 1) + "，属于超重类别。建议减少高热量食物摄入，并增加日常活动量。";
            } else {
                assessment += "您的体质指数 (BMI) 为 " + QString::number(bmi, 'f', 1) + "，属于肥胖类别。建议与医疗专业人员沟通，制定科学的减肥计划，并定期监测身体状况。";
            }
            break;
    }

    // 肺活量评估
    switch (Sexstr == "男" ? 1 : 2)  {
        case 1:
            if (vital_capacityint < 3500) {
                assessment += "您的肺活量偏低，建议加强呼吸功能锻炼，必要时请咨询医生。";
            } else if (vital_capacityint >= 3500 && vital_capacityint <= 5500) {
                assessment += "您的肺活量在正常范围内，继续保持良好的运动习惯。";
            } else {
                assessment += "您的肺活量高于正常水平，可能与良好的肺部健康或高强度的体育锻炼有关。";
            }
            break;
        case 2:
            if (vital_capacityint < 3000) {
                assessment += "您的肺活量偏低，建议加强呼吸功能锻炼，必要时请咨询医生。";
            } else if (vital_capacityint >= 3000 && vital_capacityint <= 5000) {
                assessment += "您的肺活量在正常范围内，继续保持良好的运动习惯。";
            } else {
                assessment += "您的肺活量高于正常水平，可能与良好的肺部健康或高强度的体育锻炼有关。";
            }
            break;
    }

    // 血压评估
    if (pressureint_height < 90 || pressureint_low < 60) {
        assessment += "您的血压偏低，建议您定期监测并咨询医生，以避免潜在的健康风险。";
    } else if (pressureint_height >= 90 && pressureint_height <= 120 && pressureint_low >= 60 && pressureint_low <= 80) {
        assessment += "您的血压在正常范围内，继续保持健康的生活方式。";
    } else {
        assessment += "您的血压偏高，建议减少盐分摄入，保持规律运动，并定期监测血压。";
    }

    // 心率评估
    if (heart_rateint < 60) {
        assessment += "您的心率过慢，可能与身体健康状况有关，建议咨询医生。";
    } else if (heart_rateint >= 60 && heart_rateint <= 100) {
        assessment += "您的心率正常，保持良好的健康习惯。";
    } else {
        assessment += "您的心率过快，可能与压力或身体状况有关，建议放松心情并咨询医生。";
    }

    // 体温评估
    if (temperatureint < 36.1) {
        assessment += "您的体温偏低，可能存在代谢问题，建议咨询医生。";
    } else if (temperatureint >= 36.1 && temperatureint <= 37.2) {
        assessment += "您的体温正常，继续保持健康的生活习惯。";
    } else {
        assessment += "您的体温偏高，可能存在感染或其他健康问题，建议立即就医。";
    }

    return assessment;
}


void healthtest::on_healthtest_sure_clicked()
{
    sexstr = ui->sex_->text();
    QString heightstr = ui->height->text();
    QString weightstr = ui->weight->text();
    QString heart_ratestr = ui->heart_rate->text();
    QString pressurestr_height = ui->pressure_high->text();
    QString pressurestr_low = ui->pressure_low->text();
    QString temperapurestr = ui->temperature->text();
    QString vital_capacitystr = ui->vital_capacity->text();

    // 类型转换，bool 量用于判断转换是否成功
    bool heightok, weightok, heartok, presshighok, presslowok, temperaok, vcok;
    heightint = heightstr.toInt(&heightok);
    weightint=weightstr.toInt(&weightok);
    heart_rateint= heart_ratestr.toInt(&heartok);
    pressureint_height=pressurestr_height.toInt(&presshighok);
    pressurestr_low=pressurestr_low.toInt(&presslowok);
    temperatureint= temperapurestr.toInt(&temperaok);
    vital_capacityint = vital_capacitystr.toInt(&vcok);

    QString assessment = Health_Test(heightint, weightint,
                                     heart_rateint, pressureint_height,
                                     pressureint_low, vital_capacityint,
                                     sexstr, temperatureint,
                                     heightok, weightok, heartok,
                                     presshighok, presslowok,
                                     vcok, temperaok);
    healthtestoutcome* healout = new healthtestoutcome(assessment, socket, user);
    healout->setWindowTitle(QString("健康评估结果"));
    healout->show();
    this->hide();
}
