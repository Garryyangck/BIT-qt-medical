#ifndef MESSAGETYPE_H
#define MESSAGETYPE_H


enum MessageType
{
    LOGIN,
    LOGIN_SUCCESS,
    LOGIN_FAILED,
    REGISTER,
    REGISTER_SUCCESS,
    REGISTER_FAILED,
    GET_RECEIVER, // 查看自己可以聊天的对象
    RET_RECEIVER, // 返回用户可聊天的对象
    GET_HISTORY, // 获取历史记录
    RET_HISTORY, // 返回历史记录
    SEND_MESSAGE, // 给对方发消息
    SEND_SUCCESS, // 发送成功，无论对方是否在线
    RECEIVER_OFFLINE, // 对方不在线
    RECEIVE_MESSAGE, // 收到对方的消息
    UPDATE_ISREAD, // 请求服务器修改自己的所有历史记录为“已读”
    UPDATE_ISREAD_SUCCESS, // 服务器修改完成
    QUIT, // 客户端退出，服务器需要在table中清除
    QUIT_SUCCESS, // 服务器已经将socket从table移除，客户端收到后才能退出

    CHANGE_INFO_D, // 更改医生个人信息
    CHANGE_INFO_P, // 更改患者个人信息
    CHANGE_INFO_D_SUCCESS,
    CHANGE_INFO_D_FAILED,
    CHANGE_INFO_P_SUCCESS,
    CHANGE_INFO_P_FAILED,

    SEE_DOCTOR_INFO, // 查看医生信息，只返回姓名的那种
    SEE_D_SUCCESS,

    SEE_ALL_SCHEDULE, // 医生查看所有挂号信息
    SEE_ALL_SCHEDULE_SUC,

    DOC_SEE_CASE, // 医生查看自己还没诊断的患者病历
    DOC_SEE_CASE_SUC,
    PAT_SEE_CASE, // 患者查看自己病历
    PAT_SEE_CASE_SUC,

    DOC_UPDATE_CASE, // 医生诊断病历
    DOC_UPDATE_CASE_SUC,
    DOC_UPDATE_CASE_FAI,

    // 下面这四个不用了，已经弃用
    SCHDULE_SEARCH, // 按时间查询
    SCHDULE_SEARCH_SUC,
    DEPART_SEARCH, // 按科室查询
    DEPART_SEARCH_SUC,

    GET_DOCTOR_PROFILE, // 获取医生详情
    RET_DOCTOR_PROFILE,
    GET_PATIENT_PROFILE, // 患者获取自己的个人详情
    RET_PATIENT_PROFILE,

    // 这下面的几个全都不用
    GET_ALL_DEPART, // 查看所有科室
    GET_ALL_DEPART_SUC,
    GET_DATE_BY_DEP, // 选择科室后在下拉框展示所有时间
    GET_DATE_BY_DEP_SUC,
    GET_SCHEDULE_BY_DEP_DATE, // 在大框里展示医生信息
    GET_SCHEDULE_BY_DEP_DATE_SUC,

    APPOINT, // 挂号
    APPOINT_SUC, // 挂号成功
    APPOINT_FAI, // 挂号失败

    GET_SCHEDULE, // 前端接口调用，获取排班表
    RET_SCHEDULE, // 服务器返回给前端的排班表

    SET_HEALTH, // 患者输入自己的状态，可能是insert，可能是update
    SET_HEALTH_SUSCCESS,
    GET_PATIENT_HEALTH, // 获取自己的患者的状态
    RES_PATIENT_HEALTH,
};

#endif // MESSAGETYPE_H
