using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FogEffect : MonoBehaviour
{
    [SerializeField]
    private Material postprocessMaterial;
    [SerializeField]
    private float waveSpeed;
    [SerializeField]
    private bool waveActive;

    private float waveDistance = 0;
    private Vector3 oldPosition;

    Camera m_cam;

    // Start is called before the first frame update
    void Start()
    {
        m_cam = GetComponent<Camera>();
        m_cam.depthTextureMode = m_cam.depthTextureMode | DepthTextureMode.Depth;
            //DepthTextureMode.Depth;
        postprocessMaterial.SetVector("_CameraPosition", m_cam.transform.position);
        postprocessMaterial.SetMatrix("_InverseView", m_cam.cameraToWorldMatrix);
        oldPosition = m_cam.transform.position;
    }

    // Update is called once per frame
    private void Update()
    {
        if (Vector3.Distance(oldPosition, m_cam.transform.position) > 0)
        {
            print("!!!");
            postprocessMaterial.SetVector("_CameraPosition", m_cam.transform.position);
            postprocessMaterial.SetMatrix("_InverseView", m_cam.cameraToWorldMatrix);
            oldPosition = m_cam.transform.position;
        }
    }

    //method which is automatically called by unity after the camera is done rendering
    private void OnRenderImage(RenderTexture source, RenderTexture destination)
    {
        //sync the distance from the script to the shader
      //  postprocessMaterial.SetFloat("_WaveDistance", waveDistance);
        //draws the pixels from the source texture to the destination texture
        Graphics.Blit(source, destination, postprocessMaterial);
    }
}
