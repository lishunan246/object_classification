import os
from shutil import copy, rmtree

data_path = "101_ObjectCategories"
train_path = "train"
validate_path = "validate"
for catalog in os.listdir(data_path):
    p = os.path.join(data_path, catalog)
    pics = os.listdir(p)
    t = int(len(pics) * 0.2)
    pics_train = pics[t:]
    pics_validate = pics[:t]

    train_catalog = os.path.join(train_path, catalog)
    validate_catalog = os.path.join(validate_path, catalog)

    if os.path.exists(train_catalog):
        rmtree(train_catalog)

    os.makedirs(train_catalog)

    if os.path.exists(validate_catalog):
        rmtree(validate_catalog)

    os.makedirs(validate_catalog)

    for pic in pics_train:
        copy(os.path.join(p, pic), train_catalog)

    for pic in pics_validate:
        copy(os.path.join(p, pic), validate_catalog)
