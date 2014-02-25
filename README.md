python-xattr-security
=====================

python library for linux acl,  capability

##Enabling ACL
To enable ACL, the filesystem must be mounted with the acl option, Use the following command to chekc it for ext* formatted partitions:
```shell
#tune2fs -l /dev/sdXX | grep "Default mount options" 
```


##API
###read_acl(path, option), return (version, ACL entry list) 
```shell
In [8]: _, v = acl.read_acl("xattr.so", acl.XATTR_ACCESS)

In [9]: v
Out[9]: 
[(1, 7, 4294967295),
 (2, 7, 1001),
 (4, 5, 4294967295),
 (16, 7, 4294967295),
 (32, 5, 4294967295)]

In [10]: x = acl.entries_to_descr(v)
Out[10]: 'uo::rwx,u:richard_n:rwx,go::rx,m::rwx,o::rx'
```
###write_acl(path, option, acl_tuple), acl_tuple is (version, ACL entry list)
```shell
In [11]: v = acl.descr_to_entries(x)
In [12]: v
[(1, 7, 4294967295),
 (2, 7, 1001),
 (4, 5, 4294967295),
 (16, 7, 4294967295),
 (32, 5, 4294967295)]
In [13]: acl.write_acl("xattr.so", acl.XATTR_ACCESS, (2, acl.descr_to_entries(x)))

```


