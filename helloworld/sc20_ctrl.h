#ifndef SC20_CTRL_H_
#define SC20_CTRL_H_

// LCD Related APIs

// RADIO Related APIs
int InitQLRILApi(void);
int SetupInitATCmd(void);

int GetSimCardSlotId(void);
void SetSimCardSlotId(int simId);
int SetupDataCall(void);

#endif