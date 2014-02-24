import xattr
import struct 
import pdb
import pwd, grp
from cStringIO import StringIO

POSIX_ACL_XATTR_ACCESS = "system.posix_acl_access"
POSIX_ACL_XATTR_DEFAULT = "system.posix_acl_default"
POSIX_ACL_XATTR_VERSION = 0x2

ACL_TYPE_ACCESS = 0x8000
ACL_TYPE_DEFAULT = 0x4000

#e_tag consts
ACL_USER_OBJ = 0x1
ACL_USER_USER = 0x2
ACL_GROUP_OBJ = 0x4
ACL_GROUP_GROUP = 0x8
ACL_MASK = 0x10
ACL_OTHER = 0x20

ALLOW_UID, ALLOW_GID, ALLOW_EID = range(3)

etag_dict = {
        ACL_USER_OBJ: ("uo", ALLOW_EID),
        ACL_USER_USER: ("u", ALLOW_UID),
        ACL_GROUP_OBJ: ("go", ALLOW_EID),
        ACL_GROUP_GROUP: ("g", ALLOW_GID),
        ACL_MASK: ("m", ALLOW_EID),
        ACL_OTHER: ("o", ALLOW_EID)
        }

#e_perm
ACL_READ = 0x4
ACL_WRITE = 0x2
ACL_EXECUTE = 0x1

def acl_entry_to_xattr(acl_entry_tuple):
    '''
    acl_entry_tuple = (e_tag, e_perm, e_id)
    '''
    return struct.pack("<HHI", *acl_entry_tuple)

def xattr_to_acl_entry(acl_entry_str):
    return struct.unpack("<HHI", acl_entry_str)

def read_acl(path, acl_type):
    acl = xattr.getxattr(path, acl_type)
    ver = struct.unpack("<I", acl[:4])[0]
    body = acl[4:] 
    entries = [] 
    for i in range(len(body) / 8):
        entries.append(xattr_to_acl_entry(body[i*8:(i+1)*8]))        
    return ver, entries
    
def write_acl(path, acl_type, acl_tuple): 
    ver, entries = acl_tuple   
    buf = StringIO()
    #write version
    buf.write(struct.pack("<I", ver))
    #write entries
    for e in entries:
        buf.write(acl_entry_to_xattr(e))        
    pdb.set_trace()
    xattr.setxattr(path, acl_type, buf.getvalue()) 
    buf.close()

def translate_read_acl(acl_entries):
    ret = []
    for e in acl_entries:
        tag_type, perm, eid = e 
        name = hex(eid) 
        if eid == 0xffffffff:
            name = ""
        tag_name = etag_dict[tag_type][0]
        if tag_type in (ACL_USER_OBJ, ACL_USER_USER):
            try:
                name = pwd.getpwuid(eid).pw_name 
            except KeyError:
                pass
        elif tag_type in (ACL_GROUP_OBJ, ACL_GROUP_GROUP):
            try:
                name = grp.getgrgid(eid).gr_name
            except KeyError:
                pass 
        perm_ret = "" 
        if perm & ACL_READ:
            perm_ret += "r"
        if perm & ACL_WRITE:
            perm_ret += "w"
        if perm & ACL_EXECUTE:
            perm_ret += "x"

        ret.append((tag_name, name, perm_ret))
    return ",".join([":".join(x) for x in ret])
        
def translate_write_acl(acl_str):
    ret = []
    for spec in acl_str.split(","):
        tag_name, eid, perm = spec.split(":")
        if not eid:
            eid = 0xffffffff
        found = None
        eid = str(eid)
        for k, v in etag_dict.items(): 
            if tag_name == v[0]:
                found = (k, v)                
                break
        if not found: 
            continue
        _, eid_type = found[1]
        if eid_type == ALLOW_UID:
            try:
                eid = pwd.getpwnam(eid).pw_uid
            except KeyError:
                raise Exception("user %s doesn't exist." % eid)
        elif eid_type == ALLOW_GID:
            try:
                eid = grp.getgrnam(eid).gr_gid
            except KeyError:
                raise Exception("group %s doesn't exist." % eid)
        eid = int(eid) 
        perm_ret = 0 
        if "r" in perm: 
            perm_ret |= ACL_READ
        if "w" in perm:
            perm_ret |= ACL_WRITE
        if "x" in perm:
            perm_ret |= ACL_EXECUTE
        ret.append((found[0], perm_ret, eid))                 
    return ret

def has_perm(user):
    '''
    test if your rule is right
    '''
    pass
