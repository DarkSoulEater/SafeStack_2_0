
#ifndef SAFESTACK_2_0_SRC_GLOBAL_LOGGING_H_
#define SAFESTACK_2_0_SRC_GLOBAL_LOGGING_H_

enum MsgType {eDebug, eWarning, eFatal, eInfo, eValidation, eSystem = eFatal};

void MessageLogger(MsgType msg_type, const char *msg, ...) noexcept;

void OpenMessageFiles();
void CloseMessageFiles();

#endif //SAFESTACK_2_0_SRC_GLOBAL_LOGGING_H_
