#ifndef	__SSID_H__
#define	__SSID_H__

void FTMC_MakeSSID(char *lpszSeed, char *lpszSSID, int nLen);
int FTMC_SaveSSID(char *lpszNewSSID, time_t xExpireIn);
int FTMC_DeleteSSID(char szSSID[32]);
int FTMC_IsValidSSID(char *lpszSSID, time_t xTime);
int FTMC_TouchSSID(char szSSID[32], time_t xTime);

#endif

