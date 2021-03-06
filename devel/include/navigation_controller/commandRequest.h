// Generated by gencpp from file navigation_controller/commandRequest.msg
// DO NOT EDIT!


#ifndef NAVIGATION_CONTROLLER_MESSAGE_COMMANDREQUEST_H
#define NAVIGATION_CONTROLLER_MESSAGE_COMMANDREQUEST_H


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
struct commandRequest_
{
  typedef commandRequest_<ContainerAllocator> Type;

  commandRequest_()
    : type(0)
    , x(0.0)
    , y(0.0)
    , theta(0.0)
    , is_nav(false)  {
    }
  commandRequest_(const ContainerAllocator& _alloc)
    : type(0)
    , x(0.0)
    , y(0.0)
    , theta(0.0)
    , is_nav(false)  {
  (void)_alloc;
    }



   typedef int64_t _type_type;
  _type_type type;

   typedef double _x_type;
  _x_type x;

   typedef double _y_type;
  _y_type y;

   typedef double _theta_type;
  _theta_type theta;

   typedef uint8_t _is_nav_type;
  _is_nav_type is_nav;





  typedef boost::shared_ptr< ::navigation_controller::commandRequest_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::navigation_controller::commandRequest_<ContainerAllocator> const> ConstPtr;

}; // struct commandRequest_

typedef ::navigation_controller::commandRequest_<std::allocator<void> > commandRequest;

typedef boost::shared_ptr< ::navigation_controller::commandRequest > commandRequestPtr;
typedef boost::shared_ptr< ::navigation_controller::commandRequest const> commandRequestConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::navigation_controller::commandRequest_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::navigation_controller::commandRequest_<ContainerAllocator> >::stream(s, "", v);
return s;
}


template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator==(const ::navigation_controller::commandRequest_<ContainerAllocator1> & lhs, const ::navigation_controller::commandRequest_<ContainerAllocator2> & rhs)
{
  return lhs.type == rhs.type &&
    lhs.x == rhs.x &&
    lhs.y == rhs.y &&
    lhs.theta == rhs.theta &&
    lhs.is_nav == rhs.is_nav;
}

template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator!=(const ::navigation_controller::commandRequest_<ContainerAllocator1> & lhs, const ::navigation_controller::commandRequest_<ContainerAllocator2> & rhs)
{
  return !(lhs == rhs);
}


} // namespace navigation_controller

namespace ros
{
namespace message_traits
{





template <class ContainerAllocator>
struct IsFixedSize< ::navigation_controller::commandRequest_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::navigation_controller::commandRequest_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::navigation_controller::commandRequest_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::navigation_controller::commandRequest_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::navigation_controller::commandRequest_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::navigation_controller::commandRequest_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::navigation_controller::commandRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "217ff03a15c0dbc7085b075d48d57395";
  }

  static const char* value(const ::navigation_controller::commandRequest_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x217ff03a15c0dbc7ULL;
  static const uint64_t static_value2 = 0x085b075d48d57395ULL;
};

template<class ContainerAllocator>
struct DataType< ::navigation_controller::commandRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "navigation_controller/commandRequest";
  }

  static const char* value(const ::navigation_controller::commandRequest_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::navigation_controller::commandRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "int64 type\n"
"float64 x\n"
"float64 y\n"
"float64 theta\n"
"bool is_nav\n"
;
  }

  static const char* value(const ::navigation_controller::commandRequest_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::navigation_controller::commandRequest_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.type);
      stream.next(m.x);
      stream.next(m.y);
      stream.next(m.theta);
      stream.next(m.is_nav);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct commandRequest_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::navigation_controller::commandRequest_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::navigation_controller::commandRequest_<ContainerAllocator>& v)
  {
    s << indent << "type: ";
    Printer<int64_t>::stream(s, indent + "  ", v.type);
    s << indent << "x: ";
    Printer<double>::stream(s, indent + "  ", v.x);
    s << indent << "y: ";
    Printer<double>::stream(s, indent + "  ", v.y);
    s << indent << "theta: ";
    Printer<double>::stream(s, indent + "  ", v.theta);
    s << indent << "is_nav: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.is_nav);
  }
};

} // namespace message_operations
} // namespace ros

#endif // NAVIGATION_CONTROLLER_MESSAGE_COMMANDREQUEST_H
