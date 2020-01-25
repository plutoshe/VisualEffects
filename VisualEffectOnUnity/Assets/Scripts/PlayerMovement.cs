using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerMovement : MonoBehaviour
{
    public float speedX = 1;
    public float speedZ = 1;
    public Material grassIndentMat = null;
    public float InfluenceRadius = 5.0f;

    // Start is called before the first frame update
    void Start()
    {
        if (grassIndentMat)
        {
            grassIndentMat.SetFloat("_InfluenceRadius", InfluenceRadius);
        }
    }

    // Update is called once per frame
    void Update()
    {
        Vector3 oldPosition = transform.position;
        Vector3 Offset = new Vector3(0,0,0);
        if (Input.GetKey(KeyCode.W))
        {
            Offset.z = speedZ;
        }
        if (Input.GetKey(KeyCode.S))
        {
            Offset.z = -speedZ;
        }
        if (Input.GetKey(KeyCode.A))
        {
            Offset.x = -speedX;
        }
        if (Input.GetKey(KeyCode.D))
        {
            Offset.x = speedX;
        }
        transform.position = Offset + oldPosition;
        if (grassIndentMat)
        {
            grassIndentMat.SetVector("_InfluencePosition", transform.position);
        }
    }
}
