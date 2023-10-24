using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[CreateAssetMenu(fileName="GunkyLad", menuName="GunkyLad")]
public class GunkyLadSO : ScriptableObject {

    public EnemySpawnScript Spawner;
    public int health;
    public int damage;

}
