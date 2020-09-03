// Auto-generated. Do not edit!

// (in-package serial_itri_driver.srv)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------


//-----------------------------------------------------------

class command_keyRequest {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.init = null;
    }
    else {
      if (initObj.hasOwnProperty('init')) {
        this.init = initObj.init
      }
      else {
        this.init = false;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type command_keyRequest
    // Serialize message field [init]
    bufferOffset = _serializer.bool(obj.init, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type command_keyRequest
    let len;
    let data = new command_keyRequest(null);
    // Deserialize message field [init]
    data.init = _deserializer.bool(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 1;
  }

  static datatype() {
    // Returns string type for a service object
    return 'serial_itri_driver/command_keyRequest';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '7d4dfd2ecc8e1df89b8c57312822d6f7';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    bool init
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new command_keyRequest(null);
    if (msg.init !== undefined) {
      resolved.init = msg.init;
    }
    else {
      resolved.init = false
    }

    return resolved;
    }
};

class command_keyResponse {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.ans = null;
    }
    else {
      if (initObj.hasOwnProperty('ans')) {
        this.ans = initObj.ans
      }
      else {
        this.ans = false;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type command_keyResponse
    // Serialize message field [ans]
    bufferOffset = _serializer.bool(obj.ans, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type command_keyResponse
    let len;
    let data = new command_keyResponse(null);
    // Deserialize message field [ans]
    data.ans = _deserializer.bool(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 1;
  }

  static datatype() {
    // Returns string type for a service object
    return 'serial_itri_driver/command_keyResponse';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '4c4b05f69b62d4641eb7409883f6359d';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    bool ans
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new command_keyResponse(null);
    if (msg.ans !== undefined) {
      resolved.ans = msg.ans;
    }
    else {
      resolved.ans = false
    }

    return resolved;
    }
};

module.exports = {
  Request: command_keyRequest,
  Response: command_keyResponse,
  md5sum() { return '599c74f7b589131c348db47ed14be887'; },
  datatype() { return 'serial_itri_driver/command_key'; }
};
