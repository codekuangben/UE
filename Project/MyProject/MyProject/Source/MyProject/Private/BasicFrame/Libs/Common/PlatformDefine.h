#ifndef __PlatformDefine_H
#define __PlatformDefine_H

/**
 * @brief ����ƽ̨����ͺ궨��
 */

// �Ƿ�ʹ���ⲿ�� Socket �⣬��ʹ�� UE �Լ��� Socket
#define USE_EXTERN_SOCKET
#undef USE_EXTERN_SOCKET			// ���� UE �Դ��Ŀ�

// �Ƿ�ʹ���ⲿ�� Thread �⣬��ʹ�� UE �Լ��� Thread
#define USE_EXTERN_THREAD
#undef USE_EXTERN_THREAD			// ���� UE �Դ��Ŀ�

// �Ƿ���е�Ԫ����
#define ENABLE_UNIT_TEST

// �Ƿ����
#define MSG_ENCRIPT
#undef MSG_ENCRIPT

// �Ƿ�ѹ��
#define MSG_COMPRESS
#undef MSG_COMPRESS

// �༭������
#ifdef WITH_EDITOR
	#define	MY_WITH_EDITOR
#else
	#undef	MY_WITH_EDITOR
#endif

#ifdef UE_BUILD_SHIPPING
	#define	MY_UE_BUILD_SHIPPING
#else
	#undef	MY_UE_BUILD_SHIPPING
#endif

#ifdef UE_BUILD_TEST
	#define	MY_UE_BUILD_TEST
#else
	#undef	MY_UE_BUILD_TEST
#endif

#endif