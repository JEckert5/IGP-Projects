# NavMesh notes

Mesh is baked into terrain

NavMesh process maps to all areas and objects marked as `Navigation Static`. This approximates the walkable surfaces of the level

### Location
Window > AI > Navigation

When on object, go to nav menu > Object and select nav static

Add navmesh agent component to enemy actor/object

Base Offset is offset from navmesh

## Scripting

`using UnityEngine.AI`

`NavMeshAgent object = GetComponent<NavMeshAgent>();`
Can do stuff like `object.goal = <Some Transform>`

