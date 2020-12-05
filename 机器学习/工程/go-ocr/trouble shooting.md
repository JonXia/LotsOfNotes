- no module named cv2 :

  > pip install opencv-python

- No module named 'object_detection'

  > pip install tensorflow-object-detection-api

- Could not create cudnn handle: CUDNN_STATUS_ALLOC_FAILED

  > GPU显存设置有问题，需要设置为仅在需要时申请显存。
  >
  > import tensorflow as tf 
  >
  > config = tf.compat.v1.ConfigProto(gpu_options=tf.compat.v1.GPUOptions(allow_growth=True)) 
  >
  > sess = tf.compat.v1.Session(config=config)

