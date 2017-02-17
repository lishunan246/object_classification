#include "common.h"
#include <cstdlib>
#include <QString>

extern const QString train_dir;
extern const QString validate_dir;
int main()
{
	catalog_init();
	get_vocabulary();
	get_training_data();
	trainSVM();
	evaluate(train_dir);
	system("pause");
	return 0;
}
