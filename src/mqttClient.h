
class MqttClient {
public:
    MqttClient(const char* address);
    virtual ~MqttClient();

    void connect();
    void send(const char* payload);

private:
    struct SImpl;
    SImpl* _impl;
};
