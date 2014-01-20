# Define target name
set (TARGET_NAME Box2D)

file (GLOB BOX2D_Collision_SRCS Box2D/Collision/*.cpp)
file (GLOB BOX2D_Collision_HDRS Box2D/Collision/*.h)
file (GLOB BOX2D_Shapes_SRCS Box2D/Collision/Shapes/*.cpp)
file (GLOB BOX2D_Shapes_HDRS Box2D/Collision/Shapes/*.h)
file (GLOB BOX2D_Common_SRCS Box2D/Common/*.cpp)
file (GLOB BOX2D_Common_HDRS Box2D/Common/*.h)
file (GLOB BOX2D_Dynamics_SRCS Box2D/Dynamics/*.cpp)
file (GLOB BOX2D_Dynamics_HDRS Box2D/Dynamics/*.h)
file (GLOB BOX2D_Contacts_SRCS Box2D/Dynamics/Contacts/*.cpp)
file (GLOB BOX2D_Contacts_HDRS Box2D/Dynamics/Contacts/*.h)
file (GLOB BOX2D_Joints_SRCS Box2D/Dynamics/Joints/*.cpp)
file (GLOB BOX2D_Joints_HDRS Box2D/Dynamics/Joints/*.h )

set(BOX2D_Rope_SRCS Rope/b2Rope.cpp)
set(BOX2D_Rope_HDRS Rope/b2Rope.h)
set(BOX2D_General_HDRS Box2D.h)

# Define source files
file (GLOB CPP_FILES 
    ${BOX2D_Joints_SRCS}
    ${BOX2D_Contacts_SRCS}
    ${BOX2D_Dynamics_SRCS}
    ${BOX2D_Common_SRCS}
    ${BOX2D_Shapes_SRCS}
    ${BOX2D_Collision_SRCS}
    ${BOX2D_Rope_SRCS})
    
file (GLOB H_FILES 
    ${BOX2D_General_HDRS}
    ${BOX2D_Joints_HDRS}
    ${BOX2D_Contacts_HDRS}
    ${BOX2D_Dynamics_HDRS}
    ${BOX2D_Common_HDRS}
    ${BOX2D_Shapes_HDRS}
    ${BOX2D_Collision_HDRS}
    ${BOX2D_Rope_HDRS})
    
set (SOURCE_FILES ${CPP_FILES} ${H_FILES})

source_group(Collision FILES ${BOX2D_Collision_SRCS} ${BOX2D_Collision_HDRS})
source_group(Collision\\Shapes FILES ${BOX2D_Shapes_SRCS} ${BOX2D_Shapes_HDRS})
source_group(Common FILES ${BOX2D_Common_SRCS} ${BOX2D_Common_HDRS})
source_group(Dynamics FILES ${BOX2D_Dynamics_SRCS} ${BOX2D_Dynamics_HDRS})
source_group(Dynamics\\Contacts FILES ${BOX2D_Contacts_SRCS} ${BOX2D_Contacts_HDRS})
source_group(Dynamics\\Joints FILES ${BOX2D_Joints_SRCS} ${BOX2D_Joints_HDRS})
source_group(Include FILES ${BOX2D_General_HDRS})
source_group(Rope FILES ${BOX2D_Rope_SRCS} ${BOX2D_Rope_HDRS})

# Define dependency libs
set (INCLUDE_DIRS_ONLY .)

# Setup target
setup_library ()
