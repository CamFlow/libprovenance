## Version

| Library version | Date       |
| --------------- | ---------- |
| 0.5.6           | N/A        |
| 0.5.5           | 30/10/2022 |
| 0.5.4           | 29/11/2021 |
| 0.5.3           | 05/12/2020 |
| 0.5.2           | 14/07/2020 |
| 0.5.1           | 11/07/2020 |
| 0.5.0           | 09/07/2020 |
| 0.4.11          | 23/03/2020 |
| 0.4.10          | 30/01/2020 |
| 0.4.9           | 23/08/2019 |
| 0.4.8           | 17/06/2019 |
| 0.4.7           | 29/04/2019 |
| 0.4.6           | 21/02/2019 |
| 0.4.5           | 30/01/2019 |
| 0.4.4           | 24/10/2018 |
| 0.4.3           | 21/08/2018 |
| 0.4.2           | 06/07/2018 |
| 0.4.1           | 05/06/2018 |
| 0.4.0           | 26/05/2018 |
| 0.3.10          | 27/01/2018 |
| 0.3.9           | 12/12/2017 |
| 0.3.8           | 28/09/2017 |
| 0.3.7           | 28/08/2017 |
| 0.3.6           | 27/08/2017 |
| 0.3.5           | 07/08/2017 |
| 0.3.4           | 03/08/2017 |
| 0.3.3           | 02/08/2017 |
| 0.3.2           | 23/06/2017 |
| 0.3.1           | 22/05/2017 |
| 0.3.0           | 15/04/2017 |
| 0.2.3           | 10/03/2017 |
| 0.2.2           | 09/03/2017 |
| 0.2.1           | 14/02/2017 |
| 0.2.0           | 23/01/2017 |
| 0.1.13          | 05/12/2016 |
| 0.1.12          | 14/11/2016 |
| 0.1.11          | 11/11/2016 |
| 0.1.10          | 28/10/2016 |
| 0.1.9           | 19/10/2016 |
| 0.1.8           | 04/10/2016 |
| 0.1.7           | 19/09/2016 |
| 0.1.6           | 02/09/2016 |
| 0.1.5           | 18/08/2016 |
| 0.1.4           | 18/08/2016 |
| 0.1.3           | 08/08/2016 |
| 0.1.2           | 26/05/2016 |
| 0.1.1           | 03/04/2016 |
| 0.1.0           | 28/03/2016 |

### v0.5.5
```
- Fix how file names are handled.
```

### v0.5.5
```
- Support node name attribute.
- Allow to turn versioning on/off.
- Add mechanism to set relay buffer size through configuration.
```

### v0.5.4
```
- Add function to retrieve number of graph elements dropped.
- Switch back to single relay channel support.
- Namespaces associated with task.
- Fix initialization in W3C serialization library.
- Support Fedora 35.
```

### v0.5.3
```
- Added dirty fix to handle back slash in path name in SPADE JSON output.
- Record packet length.
- Change how taints are handled.
- Remove duplicate option.
- Support Fedora 33.
```

### v0.5.2
```
- Further security context fixes.
```

### v0.5.1
```
- Fix issue where secctx is not happened to inode in SPADE format.
```

### v0.5.0
```
- Record secctx for stack in SPADE format similar to W3C output (bug fix).
- Change where dependencies are built.
```

### v0.4.11
```
- Support Fedora 31.
```

### v0.4.10
```
- Revised user provenance disclosure interface.
```

### v0.4.9
```
- Resource accounting attributes moved from proc to task.
```

### v0.4.8
```
- Release for fedora 29.
```

### v0.4.7
```
- Fix address bug.
- Record task_id (thread) as an edge attribute.
```

### v0.4.6
```
- Print service type in address label.
- In address description: serv -> service.
- Fixing getnameinfo issue (address name resolution).
```

### v0.4.5
```
- Support Ubuntu package.
- Deal with commit ID.
```

### v0.4.4
```
- Change SPADE formatting.
- Prefix epoch entry for W3C JSON.
```

### v0.4.3
```
- Small update due to underlying kernel changes.
```

### v0.4.2
```
- Handle threads affinity.
```

### v0.4.1
```
- Follow change of some entities in name in kernel.
```

### v0.4.0
```
- Support node duplication on/off.
- Added support for SPADE.
- Added support for thread level provenance.
- Accept larger security context in cache.
```

### v0.3.10
```
- Modify how IPV4 address serialisation is handled.
- Record list of loaded LSM.
```

### v0.3.9
```
- Export library version.
- Expand compression handling.
- Flags recorded for relations.
- No more flags attribute on xattr.
- Add provenance_secctx_opaque
- Add support for opaque groups and users.
- Support mutliple relay channel.
- Provide API to read CamFlow version from kernel.
```

### v0.3.8
```
- Add option to set node compression.
- Fix provenanceProvJSON.h header name.
- One hook per disclosed vertex type (entity, activity, agent).
- Distribute as a shared library.
```

### v0.3.7
```
- Remove unknown relation.
- Fix to terminate and change appearing in "relation category" (now properly in informed).
- Namespace machine id.
```

### v0.3.6
```
- Record process performance information.
- Shutdown properly.
- Reducing dependencies between kernel version and userspace software.
- Add accept_socket relationship.
- Add setattr_inode relationship.
```

### v0.3.5
```
- Update to test coverage.
```

### v0.3.4
```
- Fix forbidden character in env and arg.
```

### v0.3.3
```
- make sure log relationship are properly labeled.
- reduce amount of duplicated information between edges and vertices.
- fix issue where allowed/disallowed where reversed in relation.
- fix issue with packet not having the attribute cf:type.
```

### v0.3.2
```
- rpm ready.
```

### v0.3.1

```
- add a function to read policy hash.
- service moved to its own repository (https://github.com/CamFlow/camflow-service).
- modify API to deal with namespaces.
- record all available namespaces.
```

### v0.3.0

```
- add hooks to help with queries implementation.
- offset does not appear in JSON if the value is 0.
- add support for boot ID.
- rework the taint/label system.
```

### v0.2.3

```
- fix rare bug in JSON generation.
```

### v0.2.2

```
- move command line tool to its own repository (https://github.com/CamFlow/camflow-cli).
- use extended attribute interface to manipulate file provenance.
```

### v0.2.1

```
- performance improvement (security context cache, no logger, relay logic).
```

### v0.2.0

```
- Handle CGroup filters.
- Handle shared read and shared write.
- Handle security context filters.
- Record inode number.
- Record service pid in /run/provenance-service.pid
- Make audit file opaque.
- secctx recorded in the json output.
- secid recoreded in the json output.
- cgroup ID recorded in json output.
- Record IP packet content.
- Fix IP filter bug.
- IFC support on hold.
- Fix a number of warning during build.
- Correct relation type between packets and source/destination inodes.
- IPv4 attribute properly set in provenance graph.
- Fix prov:type attribute missing in packet nodes.
```

### v0.1.13

```
- Replace cf:type by prov:type.
- Add support for xattr (node and relations).
- Add new relations, and nodes based on LSM changes.
- Fixed duplicated node attribute type.
- Add command line option to delete ingress and egress ipv4 filter.
- Add provenance_ingress_ipv4_delete and provenance_egress_ipv4_delete.
- Replace "version" by "version_activity" and "version_entity".
```

### v0.1.12

```
- Fix bug in the command line tool (--track-file --track-process not working).
```

### v0.1.11

```
- Does not taint attribute if no taint is set.
- Relation and node types recorded as string.
- Add support for tracking on socket connect and bind.
- Propagate always imply track.
- Clarify command line tool.
- Add interface to set process tracking options.
- 64 bits integers as string in the JSON.
- Add a callback per relation types (none for agents related thing at this point).
- Rework for 64 bits types.
```

### v0.1.10

```
- Guarantee machine id is properly set.
- Add pid and vpid attribute to task JSON serialisation.
```

### v0.1.9

```
- Private mmaped files now appear as separate nodes, connected to the mmaped file by a create relationship.
- Add offset to relation if file info is set.
- Jiffies attribute in JSON output.
```

### v0.1.8

```
- Changed attribute name cf:parent_id -> cf:hasParent.
- Add infrastructure to deal with IPv4 packet provenance.
- Added prov:label elements.
```

### v0.1.7

```
- Adding API to manipulate taint.
```

### v0.1.6

```
- Rework how tracking propagation work.
- Added utils function for compression + encoding.
- Refactor code relating to relay.
```


### v0.1.5

```
- Fix bug when reading from relay.
```


### v0.1.4

```
- Allow to set tracking options on a file.
- Adding function to flush relay buffer.
- Fixing polling bug, that used a very large amount of CPU through busy wait.
- Edge renamed relation to align with W3C PROV model.
- Examples moved to https://github.com/CamFlow/examples.
- Install library to /usr/local
- Filter related prototypes now in provenancefilter.h
- Callbacks to filter provenance data in userspace.
- camflow-prov -v print version of CamFlow LSM.
```

### v0.1.3

```
- Added a command line tool to configure provenance.
- Provide functionality to create JSON output corresponding to chunk of the graph.
- Aligning with W3C Prov JSON format.
```

### v0.1.2

```
- Added functions to serialize row kernel data to json.
- Added a function to verify the presence of the IFC module in the kernel.
```

### v0.1.1

```
- IFC Security context recorded in audit.
```

### v0.1.0

```
- Initial release.
```
