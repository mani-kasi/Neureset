#ifndef ELECTRODE_H
#define ELECTRODE_H

#include <QObject>

class Electrode : public QObject
{
    Q_OBJECT
public:
    explicit Electrode(QObject *parent = nullptr);

signals:

};

#endif // ELECTRODE_H
