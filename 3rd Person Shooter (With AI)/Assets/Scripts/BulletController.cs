using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BulletController : MonoBehaviour
{
    [SerializeField]
    private GameObject bulletDecal;

    private GameObject parent;

    private float speed = 50f;
    private float timeToDestroy = 3f;

    public Vector3 target { get; set; }
    public bool hit { get; set; }
    
    private void OnEnable()
    {
        Destroy(gameObject, timeToDestroy);
    }

    // Update is called once per frame
    void Update()
    {
        transform.position = Vector3.MoveTowards(transform.position, target, speed * Time.deltaTime);
        
        if(!hit && Vector3.Distance(transform.position, target) < 0.01f) {
            Destroy(gameObject);
        }
    }

    private void OnCollisionEnter(Collision collision) {
        if (collision.gameObject.CompareTag(parent.tag))
            return;
        
        ContactPoint contact = collision.GetContact(0);
        Instantiate(bulletDecal, contact.point + contact.normal * 0.001f, Quaternion.LookRotation(contact.normal));
        
        if (collision.gameObject.CompareTag("Player")) {
            var glorg = collision.gameObject.GetComponent<PlayerController>();

            glorg.Damage(10);
            
            Debug.Log("Damage Player");

            Destroy(gameObject);

            return;
        }

        if (collision.gameObject.CompareTag("Enemy")) {
            var glorg = collision.gameObject.GetComponent<EnemyController>();

            if (glorg.Damage(20)) {
                parent.GetComponent<PlayerController>().AddScore();
            }
            
            Destroy(gameObject);
        }
        
    }

    public void SetParent(GameObject obj) {
        parent = obj;
    }

}
