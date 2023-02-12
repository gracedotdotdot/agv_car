// Auto-generated. Do not edit!

// (in-package navigation_controller.srv)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------


//-----------------------------------------------------------

class command_statusRequest {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.get_status = null;
      this.is_cancel = null;
      this.is_charging = null;
    }
    else {
      if (initObj.hasOwnProperty('get_status')) {
        this.get_status = initObj.get_status
      }
      else {
        this.get_status = false;
      }
      if (initObj.hasOwnProperty('is_cancel')) {
        this.is_cancel = initObj.is_cancel
      }
      else {
        this.is_cancel = false;
      }
      if (initObj.hasOwnProperty('is_charging')) {
        this.is_charging = initObj.is_charging
      }
      else {
        this.is_charging = false;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type command_statusRequest
    // Serialize message field [get_status]
    bufferOffset = _serializer.bool(obj.get_status, buffer, bufferOffset);
    // Serialize message field [is_cancel]
    bufferOffset = _serializer.bool(obj.is_cancel, buffer, bufferOffset);
    // Serialize message field [is_charging]
    bufferOffset = _serializer.bool(obj.is_charging, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type command_statusRequest
    let len;
    let data = new command_statusRequest(null);
    // Deserialize message field [get_status]
    data.get_status = _deserializer.bool(buffer, bufferOffset);
    // Deserialize message field [is_cancel]
    data.is_cancel = _deserializer.bool(buffer, bufferOffset);
    // Deserialize message field [is_charging]
    data.is_charging = _deserializer.bool(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 3;
  }

  static datatype() {
    // Returns string type for a service object
    return 'navigation_controller/command_statusRequest';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'd18fcf9db4fe2d25e7db577dfd268f53';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    bool get_status
    bool is_cancel
    bool is_charging
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new command_statusRequest(null);
    if (msg.get_status !== undefined) {
      resolved.get_status = msg.get_status;
    }
    else {
      resolved.get_status = false
    }

    if (msg.is_cancel !== undefined) {
      resolved.is_cancel = msg.is_cancel;
    }
    else {
      resolved.is_cancel = false
    }

    if (msg.is_charging !== undefined) {
      resolved.is_charging = msg.is_charging;
    }
    else {
      resolved.is_charging = false
    }

    return resolved;
    }
};

class command_statusResponse {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.status_is = null;
    }
    else {
      if (initObj.hasOwnProperty('status_is')) {
        this.status_is = initObj.status_is
      }
      else {
        this.status_is = '';
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type command_statusResponse
    // Serialize message field [status_is]
    bufferOffset = _serializer.string(obj.status_is, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type command_statusResponse
    let len;
    let data = new command_statusResponse(null);
    // Deserialize message field [status_is]
    data.status_is = _deserializer.string(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += object.status_is.length;
    return length + 4;
  }

  static datatype() {
    // Returns string type for a service object
    return 'navigation_controller/command_statusResponse';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'f2ddd7fe0fa22f73d06594e507a4a697';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    string status_is
    
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new command_statusResponse(null);
    if (msg.status_is !== undefined) {
      resolved.status_is = msg.status_is;
    }
    else {
      resolved.status_is = ''
    }

    return resolved;
    }
};

module.exports = {
  Request: command_statusRequest,
  Response: command_statusResponse,
  md5sum() { return '33ed9e193eaee162a578945b5dc27255'; },
  datatype() { return 'navigation_controller/command_status'; }
};
