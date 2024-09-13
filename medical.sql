CREATE TABLE user (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT NOT NULL UNIQUE,
    password TEXT NOT NULL,
    role INTEGER NOT NULL
);

CREATE TABLE patient (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL UNIQUE,
    name TEXT NOT NULL UNIQUE,
    gender TEXT,
    age TEXT,
    phone_number TEXT
);

CREATE TABLE doctor (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL UNIQUE,
    name TEXT NOT NULL UNIQUE,
    gender TEXT,
    age TEXT,
    phone_number TEXT,
    department TEXT
);

-----------------------------------------------------------

# 排班表
CREATE TABLE schedule (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    doctor_name TEXT NOT NULL,
    department TEXT NOT NULL,
    schedule_date DATE NOT NULL,
    available_num INTEGER NOT NULL
);

# 预约_病例表
# symptom 症状
# diagnose 医嘱
# medicine 处方
# isDialogue 是否已诊断
# 编辑病例和编辑处方合一块
CREATE TABLE appointment_case (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    patient_name TEXT NOT NULL,
    doctor_name TEXT NOT NULL,
    appointment_date DATE NOT NULL,
    department TEXT NOT NULL,
    symptoms TEXT,
    diagnosis TEXT,
    medicine TEXT,
    isDialogue INTEGER DEFAULT 0
);

# 医患沟通记录表
# isRead，0为未读，1为已读
CREATE TABLE chat_message (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    sender_name TEXT NOT NULL,
    receiver_name TEXT NOT NULL,
    chat_date DATE NOT NULL,
    content TEXT NOT NULL,
    isRead INTEGER DEFAULT 0
);

# 健康评估表，每个指标有一个正常范围
CREATE TABLE health (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    patient_name TEXT,
    height TEXT,
    weight TEXT,
    heart_rate TEXT,
    lung_capacity TEXT,
    systolic TEXT,
    diastolic TEXT
);
