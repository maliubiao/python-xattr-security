# !/usr/bin/env python
import xattr
import struct
import sys
import pdb
from cStringIO import StringIO

XATTR_CAP = "security.capability" 

CAP_EFFECTIVE = 0x1


#POSIX-draft defined capabilities 
"""
This overrides the restriction of changin file ownership and group ownership
"""
CAP_CHOWN = 0

"""
Override all DAC access , including ACL execute access if [_POSIX_ACL] is defined. Excluding DAC access covered by CAP_LINUX_IMMUTABLE"
"""
CAP_DAC_OVERRIDE = 1

"""
Override all dac restrictions regarding read and search on files and directories, including ACL restrictions if [_POSIX_ACL] is defined
"""
CAP_DAC_READ_SEARCH = 2 

"""
Overrides all restrictions about allowed operations on files, where
file owner ID must be equal to the user ID, except where CAP_FSETID
is applicable. It doesn't override MAC and DAC restrictions.
"""
CAP_FOWNER = 3

"""
Overrides the following restrictions that the effective user ID
shall match the file owner ID when setting the S_ISUID and S_ISGID
bits on that file; that the effective group ID (or one of the
supplementary group IDs) shall match the file owner ID when setting
the S_ISGID bit on that file; that the S_ISUID and S_ISGID bits are
cleared on successful return from chown(2) (not implemented).
""" 
CAP_FSETID = 4

"""
Overrides the restriction that the real or effective user ID of a
process sending a signal must match the real or effective user ID
of the process receiving the signal. 
"""
CAP_KILL = 5

"""
Allows setgid(2) manipulation
Allows setgroups(2)
Allows forged gids on socket credentials passing. 
""" 
CAP_SETGID = 6


"""
Allows set*uid(2) manipulation (including fsuid).
Allows forged pids on socket credentials passing.
""" 
CAP_SETUID = 7

"""
Without VFS support for capabilities:
Transfer any capability in your permitted set to any pid,
remove any capability in your permitted set from any pid
With VFS support for capabilities (neither of above, but)
Add any capability from current's capability bounding set
to the current process' inheritable set
Allow taking bits out of capability bounding set
Allow modification of the securebits for a process
""" 
CAP_SETPCAP =  8

"""
Allow modification of S_IMMUTABLE and S_APPEND file attributes
""" 
CAP_LINUX_IMMUTABLE = 9


"""
Allows binding to TCP/UDP sockets below 1024
Allows binding to ATM VCIs below 32
""" 
CAP_NET_BIND_SERVICE = 10

"""
Allow broadcasting, listen to multicast
""" 
CAP_NET_BROADCAST = 11

"""
Allow interface configuration
Allow administration of IP firewall, masquerading and accounting
Allow setting debug option on sockets
Allow modification of routing tables
Allow setting arbitrary process / process group ownership on
sockets
Allow binding to any address for transparent proxying (also via NET_RAW)
Allow setting TOS (type of service)
Allow setting promiscuous mode
Allow clearing driver statistics
Allow multicasting
Allow read/write of device-specific registers
Allow activation of ATM control sockets
"""

CAP_NET_ADMIN = 12

"""
Allow use of RAW sockets
Allow use of PACKET sockets
Allow binding to any address for transparent proxying (also via NET_ADMIN)
""" 
CAP_NET_RAW = 13

"""
Allow locking of shared memory segments
Allow mlock and mlockall (which doesn't really have anything to do
   with IPC)
""" 
CAP_IPC_LOCK = 14

"""
Override IPC ownership checks
""" 
CAP_IPC_OWNER = 15

"""
Insert and remove kernel modules - modify kernel without limit
"""
CAP_SYS_MODULE = 16

"""
Allow ioperm/iopl access
Allow sending USB messages to any device via /proc/bus/usb
""" 
CAP_SYS_RAWIO = 17

"""
Allow use of chroot()
""" 
CAP_SYS_CHROOT = 18

"""
Allow ptrace() of any process
""" 
CAP_SYS_PTRACE = 19

"""
Allow configuration of process accounting
""" 
CAP_SYS_PACCT = 20

"""
Allow configuration of the secure attention key
Allow administration of the random device
Allow examination and configuration of disk quotas
Allow setting the domainname
Allow setting the hostname
Allow calling bdflush()
Allow mount() and umount(), setting up new smb connection
Allow some autofs root ioctls
Allow nfsservctl
Allow VM86_REQUEST_IRQ
Allow to read/write pci config on alpha
Allow irix_prctl on mips (setstacksize)
Allow flushing all cache on m68k (sys_cacheflush)
Allow removing semaphores
Used instead of CAP_CHOWN to "chown" IPC message queues, semaphores
   and shared memory
Allow locking/unlocking of shared memory segment
Allow turning swap on/off
Allow forged pids on socket credentials passing
Allow setting readahead and flushing buffers on block devices
Allow setting geometry in floppy driver
Allow turning DMA on/off in xd driver
Allow administration of md devices (mostly the above, but some
   extra ioctls)
Allow tuning the ide driver
Allow access to the nvram device
Allow administration of apm_bios, serial and bttv (TV) device
Allow manufacturer commands in isdn CAPI support driver
Allow reading non-standardized portions of pci configuration space
Allow DDI debug ioctl on sbpcd driver
Allow setting up serial ports
Allow sending raw qic-117 commands
Allow enabling/disabling tagged queuing on SCSI controllers and sending
   arbitrary SCSI commands
Allow setting encryption key on loopback filesystem
Allow setting zone reclaim policy
"""
CAP_SYS_ADMIN = 21

""" 
Allow use of reboot()
""" 
CAP_SYS_BOOT = 22

"""
Allow raising priority and setting priority on other (different
   UID) processes */
Allow use of FIFO and round-robin (realtime) scheduling on own
   processes and setting the scheduling algorithm used by another
   process. */
Allow setting cpu affinity on other processes */
""" 
CAP_SYS_NICE = 23

"""
Override resource limits. Set resource limits.
Override quota limits.
Override reserved space on ext2 filesystem
Modify data journaling mode on ext3 filesystem (uses journaling
   resources)
NOTE: ext2 honors fsuid when checking for resource overrides, so
   you can override using fsuid too
Override size restrictions on IPC message queues
Allow more than 64hz interrupts from the real-time clock
Override max number of consoles on console allocation
Override max number of keymaps
"""
CAP_SYS_RESOURCE = 24

"""
Allow manipulation of system clock
Allow irix_stime on mips
Allow setting the real-time clock
""" 
CAP_SYS_TIME = 25

"""
Allow configuration of tty devices
Allow vhangup() of tty
"""
CAP_SYS_TTY_CONFIG = 26

"""
Allow the privileged aspects of mknod()
""" 
CAP_MKNOD = 27

"""
Allow taking of leases on files
""" 
CAP_LEASE = 28

CAP_AUDIT_WRITE = 29

CAP_AUDIT_CONTROL = 30

CAP_SETFCAP	= 31

"""
Override MAC access.
The base kernel enforces no MAC policy.
An LSM may enforce a MAC policy, and if it does and it chooses
to implement capability based overrides of that policy, this is
the capability it should use to do so.
"""
CAP_MAC_OVERRIDE = 32

"""
Allow MAC configuration or state changes.
The base kernel requires no MAC configuration.
An LSM may enforce a MAC policy, and if it does and it chooses
to implement capability based checks on modifications to that
policy or the data required to maintain it, this is the
capability it should use to do so.
"""
CAP_MAC_ADMIN = 33

"""
Allow configuring the kernel's syslog (printk behaviour)
""" 
CAP_SYSLOG = 34

"""
Allow triggering something that will wake the system
""" 
CAP_WAKE_ALARM = 35

"""
Allow preventing system suspends */
""" 
CAP_BLOCK_SUSPEND = 36

CAP_LAST_CAP = CAP_BLOCK_SUSPEND


#v1, v2 is too old and deprecated.  
#magic

VFS_CAP_REVISION_2 = 0x02000000


#if flag valid
cap_valid = lambda x: (x >= 0) and (x <= CAP_LAST_CAP) 
#if flag set
cap_is_set = lambda x, y: x & (1L << y)
#add flag to cap
cap_add = lambda x, y: x | (1L << y)
#remove falg from cap
cap_remove =lambda x, y: x & ~(1L << y)
"""
data structure

typedef struct kernel_cap_struct {
    /* two 32bit intergers as one 64bit
	__u32 cap[_KERNEL_CAPABILITY_U32S];
} kernel_cap_t;

struct cpu_vfs_cap_data {
	__u32 magic_etc;
	kernel_cap_t permitted;
	kernel_cap_t inheritable;
};
""" 

cap_dict = {
        "audit_control": CAP_AUDIT_CONTROL,
        "audit_write": CAP_AUDIT_WRITE,
        "block_suspend": CAP_BLOCK_SUSPEND,
        "chown": CAP_CHOWN,
        "dac_override": CAP_DAC_OVERRIDE,
        "dac_read_search": CAP_DAC_READ_SEARCH,
        "cap_fowner": CAP_FOWNER,
        "fsetid": CAP_FSETID,
        "ipc_lock": CAP_IPC_LOCK,
        "ipc_owner": CAP_IPC_OWNER,
        "kill": CAP_KILL,
        "lease": CAP_LEASE,
        "linux_immutable": CAP_LINUX_IMMUTABLE,
        "mac_admin": CAP_MAC_ADMIN,
        "mac_override": CAP_MAC_OVERRIDE,
        "mknod": CAP_MKNOD,
        "net_admin": CAP_NET_ADMIN,
        "net_bind_service": CAP_NET_BIND_SERVICE,
        "net_broadcast": CAP_NET_BROADCAST,
        "net_raw": CAP_NET_RAW,
        "setgid": CAP_SETGID,
        "setfcap": CAP_SETFCAP,
        "setpcap": CAP_SETPCAP,
        "setuid": CAP_SETUID,
        "sys_admin": CAP_SYS_ADMIN,
        "sys_boot": CAP_SYS_BOOT,
        "sys_chroot": CAP_SYS_CHROOT,
        #todo 
        }

def read_cap(path):
    cap_data = xattr.getxattr(path, XATTR_CAP) 
    magic = struct.unpack("<I", cap_data[:4])[0] 
    if magic != VFS_CAP_REVISION_2:
        raise Exception("cap v3 only")
    #little endian, 64bit

    perm = struct.unpack("<Q", cap_data[4:12])[0]
    inhert = struct.unpack("<Q", cap_data[12:])[0]
    return (perm, inhert)

def write_cap(path, capset, is_effective=True): 
    perm, inhert = capset
    cap_data = StringIO()
    #magic
    magic = VFS_CAP_REVISION_2
    if is_effective:
        magic |= CAP_EFFECTIVE 
    cap_data.write(struct.pack("<I", magic))
    #perm 
    cap_data.write(struct.pack("<Q", perm))
    #inhert
    cap_data.write(struct.pack("<Q", inhert))
    data = cap_data.getvalue() 
    cap_data.close() 
    try:
        xattr.setxattr(path, XATTR_CAP, data, xattr.XATTR_CREATE) 
    except OSError:
        xattr.setxattr(path, XATTR_CAP, data, xattr.XATTR_REPLACE) 

def print_usage():
    print "capset executable description"

def main():
    args = sys.argv
    if len(args) < 2:
        print_usage()
        exit()
    ex= args[1]
    descr = args[2] 
    perm = 0
    inhert = 0
    #format perm=cap1,cap2..|inhert=cap1,..
    for k in descr.split("|"):
        name, cap = k.split("=")
        for j in cap.split(","):
            if name == "perm": 
                perm = cap_add(perm, cap_dict[j])
            elif name == "inhert":
                inhert = cap_add(inhert, cap_dict[j])
    write_cap(ex, (perm, inhert))
        
if __name__ == "__main__":
    main()


