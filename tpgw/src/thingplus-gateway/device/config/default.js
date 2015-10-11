'use strict';
var os = require('os'),
  path = require('path');

module.exports = {
  Auth: {
    admin: '93f77339234c8e6717412baad4f53dae' //crypto.createHash('md5').update('admin:Sensor.js:admin123').digest('hex'),
  },
  Init: {
    timeout: 5 * 60 * 1000, // 5min
    disableUI: false, //disable express.js UI
  },
  Gateway: {
    name: os.hostname(),
    firstDelay: 5000,
    reportInterval: 60000,
    CERT_FILE_PATH: path.normalize(__dirname + '/..' + '/ssl/cert.p12'),
    CA_FILE_PATH: path.normalize(__dirname + '/..' + '/ssl/ca-cert.pem'),
    port: 80,
    logBaseDir: '/var/log/thingplus/',
    logName: 'thingplus.log'
  },
  Server: {
    timeSyncCheckInterval: 60 * 1000, // 1min
    mqtt: {
      protocol: 'mqtts',
      host: 'mqtt.thingplus.net',
      port: '8883',
      retryConnectInterval: 1*60*1000, //1min
      ackTimeout: 30*1000, // 30 sec
    },
    service: {
      protocol: 'https',
      host: 'www.thingplus.net',
      port: '8443'
    },
    APIKEY: ''
  },
  Store: {
    baseDir: '/var/lib/thingplus/',
    currentFile: 'db.dirty',
    cleanUpInterval: 60 * 60 * 1000, // 1hour
    logFlushFilesystemLimit: 10, //10%
    storeDisableFilesystemLimit: 3, //3%
    storeDisableMemoryLimit: 0, //0%
    rebootInOfflineInterval: 6*3600, //6 hours
    memoryOnly: false,
  },
  Sensors: {
  },
  Modem: {
  },
  Wlan: {
  },
  DM: {
    poweroff: {
      shellCmd: 'sh ./update/poweroff.sh &',
      support: true,
    },
    reboot: {
      shellCmd: 'sh ./update/reboot.sh &',
      support: true,
    },
    restart: {
      shellCmd: 'sh ./update/restart.sh &',
      support: true,
    },
    swUpdate: {
      shellCmd: 'sh ./update/update.sh &',
      support: true,
    },
    swInfo: {
      shellCmd: 'sh ./update/version.sh &',
      support: true,
    }
  },
};
