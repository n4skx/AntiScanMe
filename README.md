# Anti Scan Me
Simple "PoC", bypass EDR/XDR looking at functions in ntdll

## TODO
| Technique | Status |
| --- | --- |
| Check IAT Hook | Not implemented |
| Check Amsi Scan | Not implemented |
| Check parent processes | Not implemented |
| Check if process is debugged | Implemented |
| Check `jmp`,`call`,`ret` | Partially implemented |
| Check if function address is in the same range than ntdll | Implemented |

## Know issues
> Make use of CreateToolhelp32Snapshot 

> Not mapping file to memory
