
unsigned char m_PC_processbuf[64];
unsigned char m_nIndexStart_pc=0;
unsigned char m_nIndex_pc=0;

void GetPC_Command(void){
  
  unsigned char  ch;

  while(1) 
  {
    if ( m_nStart_pc != m_nEnd_pc )
    {
      m_nEnd_pc %= 64;
      ch = m_PC_tempbuf[m_nEnd_pc++];

      switch(ch)
      {
      
      case '$':
        m_nIndexStart_pc = 1;
        m_nIndex_pc = 0;
        m_PC_processbuf[m_nIndex_pc++] = ch;
        break;

      case '\r':
        m_PC_processbuf[m_nIndex_pc++] = 0x00;

        if(m_nIndexStart_pc==1)
          PC_CommandParse(m_PC_processbuf);
        m_nIndexStart_pc =0;
        m_nIndex_pc =0;
        break;

      default:
        if (m_nIndexStart_pc==1)
          m_PC_processbuf[m_nIndex_pc++] = ch;
        break;

      }

      if (m_nIndex_pc>64)
      {
        m_nIndexStart_pc = 0;
        m_nIndex_pc = 0;
        break;
      }
    }
    else break;
  }
  
}

void PC_CommandParse(unsigned char *buf){
 
  if ( strstr((char *)buf,"STEMON_admin_1219")!=0){
    //ProcessSSID(buf,1);
    Serial.println("      !!!! ADMIN MODE");
    GO_TO_ADMIN_MODE();

  }
  else if ( strstr((char *)buf,"STEMON_main_0505")!=0){
    //ProcessSSID(buf,1);
    Serial.println("      !!!! Return Operating MODE");
    GO_TO_STAND_BY_MODE(MAIN_STAND_BY_MODE);

  }

 

}
