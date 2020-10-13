#include "cloudtest.h"
#include "itemdrawabletest.h"
#include "steptest.h"

int main( int argc, char *argv[])
{
    CloudTest ct;
    QTest::qExec (&ct, argc, argv );
    ItemDrawableTest idt ;
    QTest::qExec (&idt, argc, argv );

    QApplication a(argc, argv);

    StepTest st ;
    QTest::qExec (&st, argc, argv );

    return 0;
}
