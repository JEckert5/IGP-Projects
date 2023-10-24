using System.Collections;
using System.Collections.Generic;
using UnityEngine;
// ReSharper disable ArrangeTypeMemberModifiers

[CreateAssetMenu(fileName="Enemy Spawner", menuName="spawner")]
public class SpawnerSO : ScriptableObject {
    private int mEnemiesSpawned = 0;
    public ScriptableObject Enemy;
    public new string name;
}
