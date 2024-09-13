# BIT-qt-medical
北理工BIT大三小学期医疗系统。

采用 QT5.8.12 + qml 实现。

---

使用方法：

1. 使用 `medical.sql` 创建数据表。

2. 将 `medical.db` 放到 `medical-server` 生成的工程文件夹中，即 server 中带 build 字样的文件夹。

	> 如果直接 copy 数据库，则忽略第一步。

3. `medical_client` 是使用 widget 制作的简略客户端。

4. `medical_client_uiCompleted` 是使用 qml 制作的精美客户端。

5. 先启动 server，然后启动 client 即可完成连接，支持多用户连接，以及局域网内部即时通信。

---

