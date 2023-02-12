// Generated by gencpp from file navigation_controller/command_statusResponse.msg
// DO NOT EDIT!


#ifndef NAVIGATION_CONTROLLER_MESSAGE_COMMAND_STATUSRESPONSE_H
#define NAVIGATION_CONTROLLER_MESSAGE_COMMAND_STATUSRESPONSE_H


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
struct command_statusResponse_
{
  typedef command_statusResponse_<ContainerAllocator> Type;

  command_statusResponse_()
    : status_is()  {
    }
  command_statusResponse_(const ContainerAllocator& _alloc)
    : status_is(_alloc)  {
  (void)_alloc;
    }



   typedef std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  _status_is_type;
  _status_is_type status_is;





  typedef boost::shared_ptr< ::navigation_controller::command_statusResponse_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::navigation_controller::command_statusResponse_<ContainerAllocator> const> ConstPtr;

}; // struct command_statusResponse_

typedef ::navigation_controller::command_statusResponse_<std::allocator<void> > command_statusResponse;

typedef boost::shared_ptr< ::navigation_controller::command_statusResponse > command_statusResponsePtr;
typedef boost::shared_ptr< ::navigation_controller::command_statusResponse const> command_statusResponseConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::navigation_controller::command_statusResponse_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::navigation_controller::command_statusResponse_<ContainerAllocator> >::stream(s, "", v);
return s;
}


template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator==(const ::navigation_controller::command_statusResponse_<ContainerAllocator1> & lhs, const ::navigation_controller::command_statusResponse_<ContainerAllocator2> & rhs)
{
  return lhs.status_is == rhs.status_is;
}

template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator!=(const ::navigation_controller::command_statusResponse_<ContainerAllocator1> & lhs, const ::navigation_controller::command_statusResponse_<ContainerAllocator2> & rhs)
{
  return !(lhs == rhs);
}


} // namespace navigation_controller

namespace ros
{
namespace message_traits
{





template <class ContainerAllocator>
struct IsFixedSize< ::navigation_controller::command_statusResponse_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::navigation_controller::command_statusResponse_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::navigation_controller::command_statusResponse_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::navigation_controller::command_statusResponse_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::navigation_controller::command_statusResponse_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::navigation_controller::command_statusResponse_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::navigation_controller::command_statusResponse_<ContainerAllocator> >
{
  static const char* value()
  {
    return "f2ddd7fe0fa22f73d06594e507a4a697";
  }

  static const char* value(const ::navigation_controller::command_statusResponse_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0xf2ddd7fe0fa22f73ULL;
  static const uint64_t static_value2 = 0xd06594e507a4a697ULL;
};

template<class ContainerAllocator>
struct DataType< ::navigation_controller::command_statusResponse_<ContainerAllocator> >
{
  static const char* value()
  {
    return "navigation_controller/command_statusResponse";
  }

  static const char* value(const ::navigation_controller::command_statusResponse_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::navigation_controller::command_statusResponse_<ContainerAllocator> >
{
  static const char* value()
  {
    return "string status_is\n"
"\n"
;
  }

  static const char* value(const ::navigation_controller::command_statusResponse_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::navigation_controller::command_statusResponse_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.status_is);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct command_statusResponse_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::navigation_controller::command_statusResponse_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::navigation_controller::command_statusResponse_<ContainerAllocator>& v)
  {
    s << indent << "status_is: ";
    Printer<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::stream(s, indent + "  ", v.status_is);
  }
};

} // namespace message_operations
} // namespace ros

#endif // NAVIGATION_CONTROLLER_MESSAGE_COMMAND_STATUSRESPONSE_H
