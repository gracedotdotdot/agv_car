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

class commandRequest {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.type = null;
      this.x = null;
      this.y = null;
      this.theta = null;
      this.is_nav = null;
    }
    else {
      if (initObj.hasOwnProperty('type')) {
        this.type = initObj.type
      }
      else {
        this.type = 0;
      }
      if (initObj.hasOwnProperty('x')) {
        this.x = initObj.x
      }
      else {
        this.x = 0.0;
      }
      if (initObj.hasOwnProperty('y')) {
        this.y = initObj.y
      }
      else {
        this.y = 0.0;
      }
      if (initObj.hasOwnProperty('theta')) {
        this.theta = initObj.theta
      }
      else {
        this.theta = 0.0;
      }
      if (initObj.hasOwnProperty('is_nav')) {
        this.is_nav = initObj.is_nav
      }
      else {
        this.is_nav = false;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type commandRequest
    // Serialize message field [type]
    bufferOffset = _serializer.int64(obj.type, buffer, bufferOffset);
    // Serialize message field [x]
    bufferOffset = _serializer.float64(obj.x, buffer, bufferOffset);
    // Serialize message field [y]
    bufferOffset = _serializer.float64(obj.y, buffer, bufferOffset);
    // Serialize message field [theta]
    bufferOffset = _serializer.float64(obj.theta, buffer, bufferOffset);
    // Serialize message field [is_nav]
    bufferOffset = _serializer.bool(obj.is_nav, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type commandRequest
    let len;
    let data = new commandRequest(null);
    // Deserialize message field [type]
    data.type = _deserializer.int64(buffer, bufferOffset);
    // Deserialize message field [x]
    data.x = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [y]
    data.y = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [theta]
    data.theta = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [is_nav]
    data.is_nav = _deserializer.bool(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 33;
  }

  static datatype() {
    // Returns string type for a service object
    return 'navigation_controller/commandRequest';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '217ff03a15c0dbc7085b075d48d57395';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    int64 type
    float64 x
    float64 y
    float64 theta
    bool is_nav
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new commandRequest(null);
    if (msg.type !== undefined) {
      resolved.type = msg.type;
    }
    else {
      resolved.type = 0
    }

    if (msg.x !== undefined) {
      resolved.x = msg.x;
    }
    else {
      resolved.x = 0.0
    }

    if (msg.y !== undefined) {
      resolved.y = msg.y;
    }
    else {
      resolved.y = 0.0
    }

    if (msg.theta !== undefined) {
      resolved.theta = msg.theta;
    }
    else {
      resolved.theta = 0.0
    }

    if (msg.is_nav !== undefined) {
      resolved.is_nav = msg.is_nav;
    }
    else {
      resolved.is_nav = false
    }

    return resolved;
    }
};

class commandResponse {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.is_running = null;
      this.run_completed = null;
    }
    else {
      if (initObj.hasOwnProperty('is_running')) {
        this.is_running = initObj.is_running
      }
      else {
        this.is_running = false;
      }
      if (initObj.hasOwnProperty('run_completed')) {
        this.run_completed = initObj.run_completed
      }
      else {
        this.run_completed = false;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type commandResponse
    // Serialize message field [is_running]
    bufferOffset = _serializer.bool(obj.is_running, buffer, bufferOffset);
    // Serialize message field [run_completed]
    bufferOffset = _serializer.bool(obj.run_completed, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type commandResponse
    let len;
    let data = new commandResponse(null);
    // Deserialize message field [is_running]
    data.is_running = _deserializer.bool(buffer, bufferOffset);
    // Deserialize message field [run_completed]
    data.run_completed = _deserializer.bool(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 2;
  }

  static datatype() {
    // Returns string type for a service object
    return 'navigation_controller/commandResponse';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '372d569423bff8e1e22d8bef0a19c178';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    bool is_running
    bool run_completed
    
    
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new commandResponse(null);
    if (msg.is_running !== undefined) {
      resolved.is_running = msg.is_running;
    }
    else {
      resolved.is_running = false
    }

    if (msg.run_completed !== undefined) {
      resolved.run_completed = msg.run_completed;
    }
    else {
      resolved.run_completed = false
    }

    return resolved;
    }
};

module.exports = {
  Request: commandRequest,
  Response: commandResponse,
  md5sum() { return 'f75d6c7ceee1c8023f1b356191a18d49'; },
  datatype() { return 'navigation_controller/command'; }
};
