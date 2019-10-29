import numpy as np
import h5py
    
    
def load_dataset():
    train_dataset = h5py.File('datasets/train_catvnoncat.h5', "r") #里面的r表示读取数据 共209张图片，h5py.File函数也可以用于写数据，将r改成w即可
    train_set_x_orig = np.array(train_dataset["train_set_x"][:]) # your train set features 原始训练集（209*64*64*3）
    train_set_y_orig = np.array(train_dataset["train_set_y"][:]) # your train set labels  原始训练集的标签集（y=0非猫,y=1是猫）（209*1）

    test_dataset = h5py.File('datasets/test_catvnoncat.h5', "r")
    test_set_x_orig = np.array(test_dataset["test_set_x"][:]) # your test set features 原始测试集（50*64*64*3）
    test_set_y_orig = np.array(test_dataset["test_set_y"][:]) # your test set labels 原始测试集的标签集（y=0非猫,y=1是猫）（50*1）

    classes = np.array(test_dataset["list_classes"][:]) # the list of classes
    
    train_set_y_orig = train_set_y_orig.reshape((1, train_set_y_orig.shape[0])) #对训练集和测试集标签进行reshape设为（1*209）
    test_set_y_orig = test_set_y_orig.reshape((1, test_set_y_orig.shape[0])) #原始测试集的标签集设为（1*50）
    
    return train_set_x_orig, train_set_y_orig, test_set_x_orig, test_set_y_orig, classes
