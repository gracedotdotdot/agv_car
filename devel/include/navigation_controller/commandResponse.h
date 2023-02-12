// Generated by gencpp from file navigation_controller/commandResponse.msg
// DO NOT EDIT!


#ifndef NAVIGATION_CONTROLLER_MESSAGE_COMMANDRESPONSE_H
#define NAVIGATION_CONTROLLER_MESSAGE_COMMANDRESPONSE_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace navigation_controller
{
template <class ContainerAllocator>
struct commandResponse_
{
  typedef commandResponse_<ContainerAllocator> Type;

  commandResponse_()
    : is_running(false)
    , run_completed(false)  {
    }
  commandResponse_(const ContainerAllocator& _alloc)
    : is_running(false)
    , run_completed(false)  {
  (void)_alloc;
    }



   typedef uint8_t _is_running_type;
  _is_running_type is_running;

   typedef uint8_t _run_completed_type;
  _run_completed_type run_completed;





  typedef boost::shared_ptr< ::navigation_controller::commandResponse_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::navigation_controller::commandResponse_<ContainerAllocator> const> ConstPtr;

}; // struct commandResponse_

typedef ::navigation_controller::commandResponse_<std::allocator<void> > commandResponse;

typedef boost::shared_ptr< ::navigation_controller::commandResponse > commandResponsePtr;
typedef boost::shared_ptr< ::navigation_controller::commandResponse const> commandResponseConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::navigation_controller::commandResponse_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::navigation_controller::commandResponse_<ContainerAllocator> >::stream(s, "", v);
return s;
}


template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator==(const ::navigation_controller::commandResponse_<ContainerAllocator1> & lhs, const ::navigation_controller::commandResponse_<ContainerAllocator2> & rhs)
{
  return lhs.is_running == rhs.is_running &&
    lhs.run_completed == rhs.run_completed;
}

template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator!=(const ::navigation_controller::commandResponse_<ContainerAllocator1> & lhs, const ::navigation_controller::commandResponse_<ContainerAllocator2> & rhs)
{
  return !(lhs == rhs);
}


} // namespace navigation_controller

namespace ros
{
namespace message_traits
{





template <class ContainerAllocator>
struct IsFixedSize< ::navigation_controller::commandResponse_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::navigation_controller::commandResponse_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::navigation_controller::commandResponse_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::navigation_controller::commandResponse_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::navigation_controller::commandResponse_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::navigation_controller::commandResponse_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::navigation_controller::commandResponse_<ContainerAllocator> >
{
  static const char* value()
  {
    return "372d569423bff8e1e22d8bef0a19c178";
  }

  static const char* value(const ::navigation_controller::commandResponse_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x372d569423bff8e1ULL;
  static const uint64_t static_value2 = 0xe22d8bef0a19c178ULL;
};

template<class ContainerAllocator>
struct DataType< ::navigation_controller::commandResponse_<ContainerAllocator> >
{
  static const char* value()
  {
    return "navigation_controller/commandResponse";
  }

  static const char* value(const ::navigation_controller::commandResponse_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::navigation_controller::commandResponse_<ContainerAllocator> >
{
  static const char* value()
  {
    return "bool is_running\n"
"bool run_completed\n"
"\n"
"\n"
;
  }

  static const char* value(const ::navigation_controller::commandResponse_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::navigation_controller::commandResponse_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.is_running);
      stream.next(m.run_completed);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct commandResponse_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::navigation_controller::commandResponse_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::navigation_controller::commandResponse_<ContainerAllocator>& v)
  {
    s << indent << "is_running: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.is_running);
    s << indent << "run_completed: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.run_completed);
  }
};

} // namespace message_operations
} // namespace ros

#endif // NAVIGATION_CONTROLLER_MESSAGE_COMMANDRESPONSE_H
