# Anti Scan Me
Simple "PoC", bypass EDR/XDR looking at functions in ntdll

## TODO
| Technique | Status |
| --- | --- |
| Check IAT Hook | Not implemented |
| Check Amsi Scan | Not implemented |
| Check parent processes | Implemented |
| Check `jmp` and `call` | Partially implemented |
| Redpill-like techniques | Partially implemented |
| Check if process is debugged | Implemented |
| Check if function address is in the same range than ntdll | Implemented |

## Know issues
> Make use of CreateToolhelp32Snapshot 
