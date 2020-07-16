#ifndef _NATIVE_WRAPPER_H_
#define _NATIVE_WRAPPER_H_

// 退出时广告
void nativeShowAdOnExit();
//界面显示前的广告
void nativeShowPopAdBefore();
// 界面显示后的广告
void nativeShowPopAdAfter();

// 更多游戏
void nativeMoreGame();

// 显示原生广告
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
