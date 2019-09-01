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

    Camera m_cam;

    // Start is called before the first frame update
    void Start()
    {
        m_cam = GetComponent<Camera>();
        m_cam.depthTextureMode = m_cam.depthTextureMode | DepthTextureMode.Depth;
    }

    // Update is called once per frame
    private void Update()
    {
        //if the wave is active, make it move away, otherwise reset it
        if (waveActive)
        {
            waveDistance = waveDistance + waveSpeed * Time.deltaTime;
        }
        else
        {
            waveDistance = 0;
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
