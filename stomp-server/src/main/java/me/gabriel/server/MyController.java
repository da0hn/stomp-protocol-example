package me.gabriel.server;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.messaging.handler.annotation.MessageMapping;
import org.springframework.messaging.handler.annotation.Payload;
import org.springframework.messaging.simp.SimpMessagingTemplate;
import org.springframework.stereotype.Controller;

/**
 * @author daohn
 * @since 24/07/2021
 */
@Controller
public class MyController {

  private final SimpMessagingTemplate template;

  @Autowired
  public MyController(SimpMessagingTemplate template) {
    this.template = template;
  }

  @MessageMapping("/input")
  public void handleReading(@Payload String inbound) {
    System.out.println(inbound);
    this.template.convertAndSend("/channel/output-2", inbound);
  }
}
