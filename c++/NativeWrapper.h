#ifndef _NATIVE_WRAPPER_H_
#define _NATIVE_WRAPPER_H_

// �˳�ʱ���
void nativeShowAdOnExit();
//������ʾǰ�Ĺ��
void nativeShowPopAdBefore();
// ������ʾ��Ĺ��
void nativeShowPopAdAfter();

// ������Ϸ
void nativeMoreGame();

// ��ʾԭ�����
void nativeShowNativeAd(int topY);
//hide native ad
void nativeHideNativeAd();

void googleRank(int score);

void nativeRate();

void nativeDevAd();

/////////////////

bool nativeIsLoadedNativeAd();
bool nativeHasOpenDevAD();

#endif
