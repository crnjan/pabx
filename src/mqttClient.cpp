
#include "mqttClient.h"

#include <cstring>
#include <MQTTAsync.h>
#include <wiringPi.h>


static void onConnectionLost(void* context, char* cause) {
    printf("Connection lost, cause %s\n", cause ? cause : "???");
    
    delay(10000);

    printf("Reconnecting\n");
    MqttClient* client = reinterpret_cast<MqttClient*>(context);
    client->connect();
}

static void onConnectFailure(void* context, MQTTAsync_failureData* response) {
    printf("Connect failed, rc %d\n", response ? response->code : 0);
}

static void onConnected(void* context, MQTTAsync_successData* response) {
    printf("Connected\n");

//    MqttClient* client = reinterpret_cast<MqttClient*>(context);
//    client->send();
}


struct MqttClient::SImpl {
    MQTTAsync _client;
};

MqttClient::MqttClient(const char* address) : _impl(new SImpl()) {
    MQTTAsync_create(&_impl->_client, address, "pabx", MQTTCLIENT_PERSISTENCE_NONE, 0);
    MQTTAsync_setCallbacks(&_impl->_client, this, onConnectionLost, 0, 0);
}

MqttClient::~MqttClient() {
    delete _impl;
}

void MqttClient::connect() {
    MQTTAsync_connectOptions connOpts = MQTTAsync_connectOptions_initializer;
    connOpts.keepAliveInterval = 20;
    connOpts.cleansession = 1;
    connOpts.onSuccess = onConnected;
    connOpts.onFailure = onConnectFailure;
    connOpts.context = this;

    int result = MQTTAsync_connect(_impl->_client, &connOpts);
    if (result != MQTTASYNC_SUCCESS) {
        printf("Connect attempt failed, rc %d\n", result);
    }
}

void MqttClient::send(const char* payload) {
    int result = MQTTAsync_send(_impl->_client, "pabx/number", strlen(payload), payload, 1, 0, 0);
    if (result != MQTTASYNC_SUCCESS) {
        printf("Sending failed, rc %d\n", result);
    }
}
