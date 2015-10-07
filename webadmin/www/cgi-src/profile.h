#ifndef	__PROFILE_H__
#define	__PROFILE_H__

#define	MAX_MODEL_LEN		64
#define	MAX_SN_LEN			32
#define	MAX_LOCATION_LEN	64
#define	MAX_USER_ID_LEN		32	
#define	MAX_PW_LEN			32
#define	MAX_ARGS			16

typedef	struct
{
	char	szModel[MAX_MODEL_LEN + 1];
	char	szSerialNumber[MAX_SN_LEN + 1];
	char	szLocation[MAX_LOCATION_LEN + 1];
	char	szUserID[MAX_USER_ID_LEN + 1];
	char	szPasswd[MAX_PW_LEN + 1];
	int		nTimeout;	
}	FTMC_PROFILE_CONFIG;

int	FTMC_LoadProfileConfig(FTMC_PROFILE_CONFIG *pConfig);
int	FTMC_SaveProfileConfig(FTMC_PROFILE_CONFIG *pConfig);

#endif

