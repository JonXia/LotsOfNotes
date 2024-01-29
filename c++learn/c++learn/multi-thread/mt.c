//#include <stdio.h>
//#include <pthread.h>
//#include <stdlib.h>
//#include <unistd.h>
//
//int g_data = 0;
//pthread_mutex_t mutex;
//void* func1(void* arg) {
//
//	printf("t1:%ld thread is create\n", (unsigned long)pthread_self());
//	printf("t1:param is %d\n", *((int*)arg));
//	pthread_mutex_lock(&mutex);
//
//	while (1) {
//		printf("t1:%d\n", g_data++);
//		sleep(1);
//		if (g_data == 3) {
//			pthread_mutex_unlock(&mutex);
//			printf("t1 quit =============================\n");
//			exit(0);
//		}
//	}
//}
//
//void* func2(void* arg) {
//
//
//	printf("t2:%ld thread is create\n", (unsigned long)pthread_self());
//	printf("t2:param is %d\n", *((int*)arg));
//	while (1) {
//		printf("t2:%d\n", g_data);
//		pthread_mutex_lock(&mutex);
//		g_data++;
//		pthread_mutex_unlock(&mutex);
//		sleep(1);
//	}
//}
//
//int main() {
//
//	int ret;
//	int param = 100;
//	pthread_t t1;
//	pthread_t t2;
//	pthread_mutex_init(&mutex, NULL);
//
//
//	ret = pthread_create(&t1, NULL, func1, (void*)&param);
//	if (ret == 0) {
//		printf("main:pthread_create success\n");
//	}
//
//	ret = pthread_create(&t2, NULL, func2, (void*)&param);
//	if (ret == 0) {
//		printf("main:pthread_create success\n");
//	}
//
//	printf("mian:%ld\n", (unsigned long)pthread_self());
//	while (1) {
//
//		printf("main:%d\n", g_data);
//		sleep(1);
//	}
//
//	pthread_join(t1, NULL);
//	pthread_join(t2, NULL);
//
//	pthread_mutex_destroy(&mutex);
//	return 0;
//
//}